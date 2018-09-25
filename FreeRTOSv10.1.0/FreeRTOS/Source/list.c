/*
 * FreeRTOS Kernel V10.1.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#include <stdlib.h>
#include "FreeRTOS.h"
#include "list.h"

/*-----------------------------------------------------------
 * PUBLIC LIST API documented in list.h
 *----------------------------------------------------------*/
	/***********************************************************************
	* 函数名称： vListInitialise
	* 函数功能： 列表初始化
	* 输入参数： pxList[OUT] : 需要被初始化的列表指针
	* 返 回 值： 无
	* 函数说明： 列表结构体中包含一个列表项成员，主要用于标记列表结束。初始化列表就是把这个列表项插入到列表中。
	****************************************************************************/

void vListInitialise( List_t * const pxList )
{
	/* The list structure contains a list item which is used to mark the
	end of the list.  To initialise the list the list end is inserted
	as the only list entry. */
	/* 列表索引指向列表项 */
	pxList->pxIndex = ( ListItem_t * ) &( pxList->xListEnd );			/*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

	/* The list end value is the highest possible value in the list to
	ensure it remains at the end of the list. */
	pxList->xListEnd.xItemValue = portMAX_DELAY;	/* 设置为最大可能值 */

	/* The list end next and previous pointers point to itself so we know
	when the list is empty. */
	/* 列表项的指针都指向自己 */
	pxList->xListEnd.pxNext = ( ListItem_t * ) &( pxList->xListEnd );	/*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */
	pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );/*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

	pxList->uxNumberOfItems = ( UBaseType_t ) 0U;

	/* Write known values into the list if
	configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
	listSET_LIST_INTEGRITY_CHECK_1_VALUE( pxList );	//列表完整性检查值设定
	listSET_LIST_INTEGRITY_CHECK_2_VALUE( pxList );
}
/*-----------------------------------------------------------*/
/***********************************************************************
* 函数名称： vListInitialiseItem
* 函数功能： 列表项初始化
* 输入参数： pxItem[OUT]: 需要初始化的列表项
* 返 回 值： 无
* 函数说明： 无
****************************************************************************/

void vListInitialiseItem( ListItem_t * const pxItem )
{
	/* Make sure the list item is not recorded as being on a list. */
	pxItem->pxContainer = NULL;	//列表项指针不指向任何列表 保证该列表项没有被任何列表包含

	/* Write known values into the list item if
	configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
	listSET_FIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE( pxItem );
	listSET_SECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE( pxItem );
}
/*-----------------------------------------------------------*/
/***********************************************************************
* 函数名称： vListInsertEnd
* 函数功能： 在列表的末尾插入一个新的列表项
* 输入参数：      pxList[IN]: 被插入的列表
			 pxNewListItem[IN]: 需要插入的新的列表项
* 返 回 值： 无
* 函数说明： 无
****************************************************************************/

void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem )
{
ListItem_t * const pxIndex = pxList->pxIndex;

	/* Only effective when configASSERT() is also defined, these tests may catch
	the list data structures being overwritten in memory.  They will not catch
	data errors caused by incorrect configuration or use of FreeRTOS. */
	listTEST_LIST_INTEGRITY( pxList );
	listTEST_LIST_ITEM_INTEGRITY( pxNewListItem );

	/* Insert a new list item into pxList, but rather than sort the list,
	makes the new list item the last item to be removed by a call to
	listGET_OWNER_OF_NEXT_ENTRY(). */
	pxNewListItem->pxNext = pxIndex;
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;

	/* Only used during decision coverage testing. */
	mtCOVERAGE_TEST_DELAY();

	pxIndex->pxPrevious->pxNext = pxNewListItem;
	pxIndex->pxPrevious = pxNewListItem;

	/* Remember which list the item is in. */
	pxNewListItem->pxContainer = pxList;

	( pxList->uxNumberOfItems )++;
}
/*-----------------------------------------------------------*/
/***********************************************************************
* 函数名称： vListInsert
* 函数功能： 将列表项插入列表中
* 输入参数： pxList[IN]: 被插入的列表
			 pxNewListItem[IN]:需要插入的列表项
* 返 回 值： 无
* 函数说明： 将列表项插入到列表中，列表项所在的位置取决于列表项的列表项值（xItemValue）
****************************************************************************/

void vListInsert( List_t * const pxList, ListItem_t * const pxNewListItem )
{
	ListItem_t *pxIterator;	//列表项中间值 用于遍历列表 寻找合适的插入位置
	const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;	//新插入列表的列表项的列表项值

	/* Only effective when configASSERT() is also defined, these tests may catch
	the list data structures being overwritten in memory.  They will not catch
	data errors caused by incorrect configuration or use of FreeRTOS. */
	listTEST_LIST_INTEGRITY( pxList );	//列表完整性检查
	listTEST_LIST_ITEM_INTEGRITY( pxNewListItem );

	/* Insert the new list item into the list, sorted in xItemValue order.

	If the list already contains a list item with the same item value then the
	new list item should be placed after it.  This ensures that TCBs which are
	stored in ready lists (all of which have the same xItemValue value) get a
	share of the CPU.  However, if the xItemValue is the same as the back marker
	the iteration loop below will not end.  Therefore the value is checked
	first, and the algorithm slightly modified if necessary. */
	/*将新列表项插入列表，按xItemValue顺序排序*/
	if( xValueOfInsertion == portMAX_DELAY ) //如果列表项的列表项值是默认最大值的话
	{
		pxIterator = pxList->xListEnd.pxPrevious; //肯定会将此列表项插入到列表的末尾 
	}
	else
	{
		/* *** NOTE ***********************************************************
		If you find your application is crashing here then likely causes are
		listed below.  In addition see https://www.freertos.org/FAQHelp.html for
		more tips, and ensure configASSERT() is defined!
		https://www.freertos.org/a00110.html#configASSERT

			1) Stack overflow -
			   see https://www.freertos.org/Stacks-and-stack-overflow-checking.html
			2) Incorrect interrupt priority assignment, especially on Cortex-M
			   parts where numerically high priority values denote low actual
			   interrupt priorities, which can seem counter intuitive.  See
			   https://www.freertos.org/RTOS-Cortex-M3-M4.html and the definition
			   of configMAX_SYSCALL_INTERRUPT_PRIORITY on
			   https://www.freertos.org/a00110.html
			3) Calling an API function from within a critical section or when
			   the scheduler is suspended, or calling an API function that does
			   not end in "FromISR" from an interrupt.
			4) Using a queue or semaphore before it has been initialised or
			   before the scheduler has been started (are interrupts firing
			   before vTaskStartScheduler() has been called?).
		**********************************************************************/
		/*遍历列表项 找到合适的插入位置*/
		for( pxIterator = ( ListItem_t * ) &( pxList->xListEnd ); pxIterator->pxNext->xItemValue <= xValueOfInsertion; pxIterator = pxIterator->pxNext ) /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. *//*lint !e440 The iterator moves to a different value, not xValueOfInsertion. */
		{
			/* There is nothing to do here, just iterating to the wanted
			insertion position. */
		}
	}

	pxNewListItem->pxNext = pxIterator->双向列表的插入 具体图解分析比较直观 文字叙述有点绕
	pxNewListItem->pxNext->pxPrevious = pxNewListItem;
	pxNewListItem->pxPrevious = pxIterator;
	pxIterator->pxNext = pxNewListItem;

	/* Remember which list the item is in.  This allows fast removal of the
	item later. */
	pxNewListItem->pxContainer = pxList;	//列表项值指向该列表

	( pxList->uxNumberOfItems )++;	//列表项值计数+1
}
/*-----------------------------------------------------------*/
/***********************************************************************
* 函数名称： uxListRemove
* 函数功能： 从列表中移除一个列表项
* 输入参数： pxItemToRemove[IN]: 需要移除的列表项
* 返 回 值： 当前列表中的列表项个数
* 函数说明： 无
****************************************************************************/

UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove )
{
/* The list item knows which list it is in.  Obtain the list from the list
item. */
List_t * const pxList = pxItemToRemove->pxContainer;	//该列表项所属的列表

	pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;	//从该列表中移除该列表项
	pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;

	/* Only used during decision coverage testing. */
	mtCOVERAGE_TEST_DELAY();

	/* Make sure the index is left pointing to a valid item. */
	if( pxList->pxIndex == pxItemToRemove )	//如果列表索引指向当前列表
	{
		pxList->pxIndex = pxItemToRemove->pxPrevious;	//则列表索引指向上一个
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	pxItemToRemove->pxContainer = NULL;	//清除列表项指向列表的指针
	( pxList->uxNumberOfItems )--;	//列表中列表项计数-1

	return pxList->uxNumberOfItems;
}
/*-----------------------------------------------------------*/

