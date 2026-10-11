void clear_screen(bool screen[kScreenWidth][kScreenHeight]);
void print_screen(bool screen[kScreenWidth][kScreenHeight]);
void load_sprite_to_mem(uint16_t addr, const uint8_t sprite[], uint8_t n);
void Annn(uint16_t nnn);
void Dxyn(uint8_t x, uint8_t y, uint8_t n);
void draw_to_screen(uint8_t x, uint8_t y, uint8_t sprite);
