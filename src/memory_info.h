#ifndef MEMORY_INFO_H_
#define MEMORY_INFO_H_

#define KEYMEMFREE  "MemFree"
#define KEYMEMTOTAL "MemTotal"
#define KEYBUFF     "Buffers"
#define KEYCACHED   "Cached"
#define KEYSWFREE   "SwapFree"
#define KEYSWTOTAL  "SwapTotal"
#define KEYSLAB     "Slab"
#define KEYMEMAVAIL "MemAvailable"

void print_memory_info(const options_t *options);

#endif /* MEMORY_INFO_H_ */
