/*
 * xboot/kernel/core/proc.c
 *
 * Copyright (c) 2007-2010  jianjun jiang <jerryjianjun@gmail.com>
 * official site: http://xboot.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <configs.h>
#include <default.h>
#include <types.h>
#include <stddef.h>
#include <string.h>
#include <macros.h>
#include <malloc.h>
#include <hash.h>
#include <vsprintf.h>
#include <xboot/list.h>
#include <xboot/initcall.h>
#include <xboot/printk.h>
#include <xboot/proc.h>


/* the list of process */
static struct proc_list __proc_list = {
	.entry = {
		.next	= &(__proc_list.entry),
		.prev	= &(__proc_list.entry),
	},
};
struct proc_list * proc_list = &__proc_list;

/*
 * search proc by name
 */
struct proc * proc_search(const char *name)
{
	struct proc_list * list;
	struct list_head * pos;

	if(!name)
		return NULL;

	for(pos = (&proc_list->entry)->next; pos != (&proc_list->entry); pos = pos->next)
	{
		list = list_entry(pos, struct proc_list, entry);
		if(strcmp((x_s8*)list->proc->name, (const x_s8 *)name) == 0)
			return list->proc;
	}

	return NULL;
}

/*
 * register a proc into proc_list
 * return true is successed, otherwise is not.
 */
x_bool proc_register(struct proc * proc)
{
	struct proc_list * list;

	list = malloc(sizeof(struct proc_list));
	if(!list || !proc)
	{
		free(list);
		return FALSE;
	}

	if(!proc->name || !proc->read || proc_search(proc->name))
	{
		free(list);
		return FALSE;
	}

	list->proc = proc;
	list_add(&list->entry, &proc_list->entry);

	return TRUE;
}

/*
 * unregister proc from proc_list
 */
x_bool proc_unregister(struct proc * proc)
{
	struct proc_list * list;
	struct list_head * pos;

	if(!proc || !proc->name)
		return FALSE;

	for(pos = (&proc_list->entry)->next; pos != (&proc_list->entry); pos = pos->next)
	{
		list = list_entry(pos, struct proc_list, entry);
		if(list->proc == proc)
		{
			list_del(pos);
			free(list);
			return TRUE;
		}
	}

	return FALSE;
}

/*
 * proc interface for self
 */
static x_s32 self_proc_read(x_u8 * buf, x_s32 offset, x_s32 count)
{
	struct proc_list * list;
	struct list_head * pos;
	x_s8 * p;
	x_s32 len = 0;

	if((p = malloc(SZ_4K)) == NULL)
		return 0;

	len += sprintf((x_s8 *)(p + len), (const x_s8 *)"[proc]");
	for(pos = (&proc_list->entry)->next; pos != (&proc_list->entry); pos = pos->next)
	{
		list = list_entry(pos, struct proc_list, entry);
		len += sprintf((x_s8 *)(p + len), (const x_s8 *)"\r\n %s", list->proc->name);
	}

	len -= offset;

	if(len < 0)
		len = 0;

	if(len > count)
		len = count;

	memcpy(buf, (x_u8 *)(p + offset), len);
	free(p);

	return len;
}

static struct proc self_proc = {
	.name	= "proc",
	.read	= self_proc_read,
};

/*
 * proc pure sync init
 */
static __init void proc_pure_sync_init(void)
{
	/* register proc interface for self */
	proc_register(&self_proc);
}

static __exit void proc_pure_sync_exit(void)
{
	/* unregister self proc interface */
	proc_unregister(&self_proc);
}

module_init(proc_pure_sync_init, LEVEL_PURE_SYNC);
module_exit(proc_pure_sync_exit, LEVEL_PURE_SYNC);
