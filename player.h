//
// Created by zeev on 10/3/21.
//

#ifndef FILES_PLAYER_H
#define FILES_PLAYER_H


class Player {
public:
    Player();
    void set_position(int x, int y);
    void init_movement(float destination_x,float destination_y,float duration);
    void integrate(long t);
    float x, y;
    int x_index, y_index, state, rotata, x_sig, z_sig, deg_state;

    void draw(int size, int i, int j);
    void init_me();
    void init_enemy(int x, int y);
    void drawTank(int x ,int y, int z);
    void adjust(int sign);
    void init_rotation(float degree, float duration);
private:float vx, vy, time_remaining, desx, desy, vdeg, desdeg;
};


#endif //FILES_PLAYER_H
