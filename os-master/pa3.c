/**********************************************************************
 * Copyright (c) 2019
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "types.h"
#include "list_head.h"
#include "vm.h"

 /**
  * Ready queue of the system
  */
extern struct list_head processes;

/**
 * The current process
 */
extern struct process* current;

/**
 * alloc_page()
 *
 * DESCRIPTION
 *   Allocate a page from the system. This function is implemented in vm.c
 *   and use to get a page frame from the system.
 *
 * RETURN
 *   PFN of the newly allocated page frame.
 */
extern unsigned int alloc_page(void);





/**
 * TODO translate()
 *
 * DESCRIPTION
 *   Translate @vpn of the @current to @pfn. To this end, walk through the
 *   page table of @current and find the corresponding PTE of @vpn.
 *   If such an entry exists and OK to access the pfn in the PTE, fill @pfn
 *   with the pfn of the PTE and return true.
 *   Otherwise, return false.
 *   Note that you should not modify any part of the page table in this function.
 *
 * RETURN
 *   @true on successful translation
 *   @false on unable to translate. This includes the case when @rw is for write
 *   and the @writable of the pte is false.
 */

bool translate(enum memory_access_type rw, unsigned int vpn, unsigned int* pfn)
{
	//���� ���μ����� ������ ���̺��� ���� VPN�� PFN (������ ������ ��ȣ)���� ��ȯ�ϴ� ���
	/*** DO NOT MODIFY THE PAGE TABLE IN THIS FUNCTION ***/
	//outer ��ȣ�� vpn/16, inner��ȣ�� vpn%16
	int outer_index = vpn / 16;
	int inner_index = vpn % 16;

	if (current->pagetable.outer_ptes[outer_index] == NULL || current->pagetable.outer_ptes[outer_index]->ptes[inner_index].valid == 0 || current->pagetable.outer_ptes[outer_index]->ptes[inner_index].writable == 0 && rw == WRITE) {
		return false;
	}

	else {
		*pfn = current->pagetable.outer_ptes[outer_index]->ptes[inner_index].pfn;
		return true;
	}

}


/**
 * TODO handle_page_fault()
 *
 * DESCRIPTION
 *   Handle the page fault for accessing @vpn for @rw. This function is called
 *   by the framework when the translate() for @vpn fails. This implies;
 *   1. Corresponding pte_directory is not exist
 *   2. pte is not valid
 *   3. pte is not writable but @rw is for write
 *   You can assume that all pages are writable; this means, when a page fault
 *   happens with valid PTE without writable permission, it was set for the
 *   copy-on-write.
 *
 * RETURN
 *   @true on successful fault handling
 *   @false otherwise
 */




void re_new(struct pte* my_inner_page, bool my_valid, bool my_wriatable) {
	my_inner_page->valid = my_valid;
	my_inner_page->writable = my_wriatable;
	my_inner_page->pfn = alloc_page();
}


bool handle_page_fault(enum memory_access_type rw, unsigned int vpn)
{
	int outer_index = vpn / 16;
	int inner_index = vpn % 16;

	if (current->pagetable.outer_ptes[outer_index] == NULL) {
		current->pagetable.outer_ptes[outer_index] = malloc(sizeof(struct pte_directory));
		re_new(&(current->pagetable.outer_ptes[outer_index]->ptes[inner_index]), 1, 1);
		return true;

	}
	if (current->pagetable.outer_ptes[outer_index]->ptes[inner_index].valid == 0 || (current->pagetable.outer_ptes[outer_index]->ptes[inner_index].writable == 0 && rw == WRITE)) {
		re_new(&(current->pagetable.outer_ptes[outer_index]->ptes[inner_index]), 1, 1);
		return true;
	}
}



/**
 * TODO switch_process()
 *
 * DESCRIPTION
 *   If there is a process with @pid in @processes, switch to the process.
 *   The @current process at the moment should be put to the **TAIL** of the
 *   @processes list, and @current should be replaced to the requested process.
 *   Make sure that the next process is unlinked from the @processes.
 *   If there is no process with @pid in the @processes list, fork a process
 *   from the @current. This implies the forked child process should have
 *   the identical page table entry 'values' to its parent's (i.e., @current)
 *   page table. Also, should update the writable bit properly to implement
 *   the copy-on-write feature.
 */
void switch_process(unsigned int pid)
{
	struct process* has_pid = NULL;
	int find = 0;




	list_for_each_entry(has_pid, &processes, list) {

		if (has_pid->pid == pid) {
			list_add_tail(&current->list, &processes);
			current = has_pid;
			list_del_init(&current->list);
			find++;
			break;
		}

	}

	if (find == 0) {
		struct process* childprocess = malloc(sizeof(struct process));


		for (int i = 0; i < 16; i++) {


			if (current->pagetable.outer_ptes[i] == NULL) {

				childprocess->pagetable.outer_ptes[i] == NULL;


			}

			else {

				childprocess->pagetable.outer_ptes[i] = malloc(sizeof(struct pte_directory));

				for (int j = 0; j < 16; j++) {

					//child ���μ����� parent ���μ��� �����ϴ�����

					childprocess->pagetable.outer_ptes[i]->ptes[j] = current->pagetable.outer_ptes[i]->ptes[j];
					childprocess->pagetable.outer_ptes[i]->ptes[j].writable = false;
					current->pagetable.outer_ptes[i]->ptes[j].writable = false;
				}



			}



		}
		childprocess->pid = pid;
		list_add_tail(&current->list, &processes);
		current = childprocess;


	}



}