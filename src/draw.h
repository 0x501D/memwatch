#ifndef DRAW_H_
#define DRAW_H_

#define ALOTOFSPACES "                                        "

#define KEYMEMFREE  "MemFree:"
#define KEYMEMTOTAL "MemTotal:"
#define KEYBUFF     "Buffers:"
#define KEYCACHED   "Cached:"
#define KEYSWFREE   "SwapFree:"
#define KEYSWTOTAL  "SwapTotal:"

void get_data(const options_t *options);

#endif /* DRAW_H_ */
