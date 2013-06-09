/*structs*/
typedef struct Ship Ship;

/*functions*/
Ship* get_ship(int x, int y);

void handle_ship_events(Ship *s, int eventType, int key);

void draw_ship(Ship *s);

void update_ship(Ship *s);

void fire_bullet();

int get_ship_vx(Ship *s);
int get_ship_vy(Ship *s);
int get_ship_vr(Ship *s);
int get_ship_rotation(Ship *s);

