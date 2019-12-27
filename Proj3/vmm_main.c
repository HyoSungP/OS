#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <assert.h>

static struct
{
  unsigned int num_page_fault;
  unsigned int num_page_try;
  unsigned int num_tlb_miss;
  unsigned int num_tlb_try;
} perf_stat = {
  .num_page_fault = 0,
  .num_page_try = 0,
  .num_tlb_miss = 0,
  .num_tlb_try = 0
};

struct tlb_entry //TLB구조
{
  uint8_t page_num;
  uint8_t frame_num;
  int is_valid; // 1 is valid or not.
};

struct page_entry //page 구조
{
  uint16_t frame_num;
  int is_valid; // 1 is valid or not.
};
int page, offset;
uint32_t phy_addr;
uint8_t lookup_tlb(uint8_t page_num);
uint8_t lookup_page_table(uint8_t page_num);
uint8_t lookup_phy_mem(uint32_t phy_addr);

void page_fault_handler(uint8_t page_num);
uint32_t to_phy_addr(uint32_t virt_addr);

struct tlb_entry tlb[16];
int tlb_fifo_idx = 0;
struct page_entry page_table[256];
uint8_t phy_mem[256 * 256] = {0,};

int main(int argc, char** argv)
{
 
  // Clean tlb and page table.
  for (int it = 0; it < 16; ++it) {
    tlb[it].is_valid = 0;
  }
  for (int it = 0; it < 256; ++it) {
    page_table[it].is_valid = 0;
  }

  uint32_t virt_addr;
  while (scanf("%u", &virt_addr) != EOF) {
    uint32_t phy_addr = to_phy_addr(virt_addr);

    fprintf(stderr, "%d\n", lookup_phy_mem(phy_addr));
  }

  printf("pf: %lf\ntlb: %lf\n",
    (double)perf_stat.num_page_fault / perf_stat.num_page_try,
    (double)perf_stat.num_tlb_miss / perf_stat.num_tlb_try);

  return 0;
}

uint8_t lookup_tlb(uint8_t page_num)
{
  perf_stat.num_tlb_try++;

  for (struct tlb_entry* it = tlb; it < tlb + 16;
       it++) {
    if (it->is_valid && it->page_num == page_num) {
      return it->frame_num;
    }
  }

  perf_stat.num_tlb_miss++;

  uint8_t frame_num = lookup_page_table(page_num);

  struct tlb_entry* it = tlb + tlb_fifo_idx;
  tlb_fifo_idx = ++tlb_fifo_idx % 16;

  it->page_num = page_num;
  it->frame_num = frame_num;
  it->is_valid = 1;

  return it->frame_num;
}

uint8_t lookup_page_table(uint8_t page_num)
{
  if (!page_table[page_num].is_valid) 
  {
    page_fault_handler(page_num);
    perf_stat.num_page_fault++;
  }

  //assert : 조건식이 거짓일떄 프로그램 중단
  assert(page_table[page_num].is_valid);

  perf_stat.num_page_try++;

  return page_table[page_num].frame_num;
}

//만약 page fault가 일어날 경우,
//BACKINGSTORE.bin 파일에서 요청된 page number 번째의 페이지 256B만큼을
//읽어 해당 번째의 physical memory의 frame에 로딩
//Page table에 frame number 기록
void page_fault_handler(uint8_t page_num)
{ 
  FILE* fp = fopen("./input/BACKINGSTORE.bin", "r");
  fseek(fp, 256 * page_num , SEEK_SET);
  fread(&phy_mem[256 * page_num], 1, 256, fp);
  page_table[page_num].frame_num = page_num;
  page_table[page_num].is_valid = 1;
  fclose(fp);
  
}

//Virtual address가 들어왔을때, 
//위 흐름도를 통해 physical memory에서의 address로 번역
uint32_t to_phy_addr(uint32_t virt_addr)
{
  uint8_t frame;
  //frame = looup(page) 여기다두면 계속 194만뜨네요..
  page = (virt_addr & 0x0000FF00) >> 8;
  frame = lookup_tlb(page); // 여기다가 두면 계속 0만뜨네요 ㅜㅠ
  offset = (virt_addr & 0x000000FF) >> 8;
  phy_addr = ((frame & 0x000000FF) << 8) + ((offset & 0x000000FF) >> 8);

  return phy_addr;
  //return 0xdeadbeaf; // TODO: Make it work!
}

//physical address 위치의 physical memory 값을 읽어서 리턴
uint8_t lookup_phy_mem(uint32_t phy_addr)
{
  offset = (phy_addr & 0x000000FF) <<8;
  uint8_t frame_num = (phy_addr & 0x000000FF) <<8;
  return phy_mem[256 * frame_num + offset];
}
