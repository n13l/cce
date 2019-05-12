/*                                                                              
 * The MIT License (MIT)                       Memory Management debug facility
 *
 * Copyright (c) 2012-2018                          Daniel Kubec <niel@rtfm.cz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"),to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in   
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef __MEM_PAGE_H__
#define __MEM_PAGE_H__

#include <sys/compiler.h>
#include <sys/cpu.h>
#include <sys/mman.h>
#include <mem/alloc.h>

#define PAGE_HDR_SHIFT  9
#define PAGE_HDR_MAGIC 0x40000000
#define PAGE_HDR_FREE  0xffffffff
#define PAGE_HDR_PART  0x80000000 /* bit mask for page parts */

__BEGIN_DECLS

struct page;
struct pages {
	u64 size;
	u32 list;             /* list of free pages */
	u32 avail;            /* number of free pages in list */
	u32 total;            /* number of pages in map                    */
	u32 shift;            /* page size aligned to power of 2           */
	struct page *page;
} _align_max;

struct page {
	u32 avail;            /* linked list of available pages            */
} _align_max;

static inline unsigned int
get_page_size(struct pages *map)
{
	return 1 << map->shift;
}

static inline struct page *
get_page(struct pages *map, unsigned int index)
{
	return (struct page*)((u8*)map->page + ((size_t)index << map->shift));
}

static inline unsigned int
page_index(struct pages *map, struct page *page)
{
	return (size_t)((u8*)page - (u8*)map->page) >> map->shift;
}

static inline unsigned long
page_offset(struct pages *pages, struct page *page)
{
	return (unsigned long)((u8*)page - (u8*)pages->page) >> pages->shift;
}

static inline bool
page_avail(struct pages *pages)
{
	return pages->list != (u32)~0U;
}

static inline struct page *
page_alloc(struct pages *pages)
{
	if (unlikely(!page_avail(pages)))
		return NULL;
	struct page *page = get_page(pages, pages->list);
	pages->list = page->avail;
	pages->avail--;
	return page;
}

static inline void
page_free(struct pages *pages, struct page *page)
{
	pages->avail = pages->list;
	pages->list = page_index(pages, page);
	pages->avail++;
}

static inline void
page_prefetch(struct page *page, u32 shift, u32 pages)
{
	u32 size = 1U << shift;
	byte addr[size], *ptr = (byte *)page;
	for (unsigned int i = 0; i < pages; i++, ptr += size)
		memcpy(addr, ptr, size);
}

static inline int
page_addr_protect(void *addr, int prot)
{
	prot = PROT_EXEC | PROT_READ;
	int rv;
	uintptr_t page = (uintptr_t)addr & ~(uintptr_t)(CPU_PAGE_SIZE - 1);
	if ((rv = mprotect((void *)page, CPU_PAGE_SIZE, prot)))
		return rv;
	if ((rv = msync((void *)page, CPU_PAGE_SIZE, MS_INVALIDATE)))
		return rv;

	return 0;
}

int
pages_alloc(struct pages *pages, int prot, int mode,
            int vm_bits, int page_bits, int total);

void
pages_reset(struct pages *pages);

int
pages_free(struct pages *pages);

_unused static unsigned long
pages2mb(u32 shift, unsigned long pages)
{
	return (pages * (1 << shift)) >> 20;
}

_unused static unsigned long
pages2b(u32 shift, unsigned long pages)
{
	return (pages * (1 << shift));
}

_unused static unsigned long
mb2pages(u32 shift, unsigned long mb)
{
	return (mb << (20 - shift));
}

#define page_for_each(map, type, p) \
  for (type p   = (type)get_page(map, 0), \
         *__x   = (type)get_page(map, map->total); \
      p < __x;p = (type)((u8*)p + (1U << map->shift)))

__END_DECLS

#endif/*__MEM_PAGE_H__*/
