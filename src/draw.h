#ifndef DRAW_H_
#define DRAW_H_

#define ALOTOFSPACES "                                        "

#define KEYMEMFREE  "MemFree"
#define KEYMEMTOTAL "MemTotal"
#define KEYBUFF     "Buffers"
#define KEYCACHED   "Cached"
#define KEYSWFREE   "SwapFree"
#define KEYSWTOTAL  "SwapTotal"
#define KEYSLAB     "Slab"

void get_data(const struct options *options);

#endif /* DRAW_H_ */
