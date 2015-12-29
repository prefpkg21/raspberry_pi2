//////////////////////////////////////////////////////////////////////
// logana.hpp -- Simple Log Analyzer Class
// Date: Tue Apr 21 22:25:37 2015   (C) Warren Gay ve3wwg
//
// Exploring the Raspberry Pi 2 with C++ (ISBN 978-1-4842-1738-2)
// by Warren Gay VE3WWG
// LGPL2 V2.1
///////////////////////////////////////////////////////////////////////

#ifndef LOGANA_HPP
#define LOGANA_HPP

#include "mailbox.hpp"
#include "rpidma.h"
#include "dma.hpp"

#include <vector>

#define LOGANA_PATH	"/dev/logana"

class LogicAnalyzer {
    uint32_t            pagespblk;  // Pages per block
    uint32_t            pagesize;   // Kernel page size
    uint32_t            sampspblk;  // Samples per block

    struct s_block {
        void            *data;      // dma mem block
        DMA::CB         *dma_cb;    // dma ctl block
        uint32_t        *samples;   // 1st sample
    };

    std::vector<s_block*> dma_blocks;

    std::vector<uint32_t>  sg_list;    // Scatter/Gather list

    int                 fd;         // /dev/rpidma
    s_rpidma_ioctl      dalloc;     // DMA Allocation
    std::string         errmsg;     // Error message
    DmaMem              dmamem;     // DMA Memory
    DMA                 dma;        // DMA registers + CB
    GPIO                gpio;       // GPIO access

public:
    LogicAnalyzer(unsigned arg_ppblk=8);
    ~LogicAnalyzer();

    inline const char *error() { return errmsg.c_str(); }
    bool open();                // Open drivers
    void close();               // Close drivers

    uint32_t get_gplev0();      // Return phys addr for GPLEV0

    bool alloc_blocks(unsigned blocks);

    bool start(unsigned long src_addr);
    int is_completed();		// 1==completed, 0==incomplete or < 0 is error

    inline size_t get_blocks() { return dma_blocks.size(); }

    uint32_t *get_samples(unsigned blockx,size_t *n_samples);
};

#endif // LOGANA_HPP

// End logana.hpp
