#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 22
#define LEVEL_HEIGHT 8

unsigned int LEVEL_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

LevelA::~LevelA() {
    delete [] m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise() {
    GLuint map_texture_id = Utility::load_texture("assets/images/tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 4, 1);
    
    // ————— TRAINER ————— //
    GLuint player_texture_id = Utility::load_texture("assets/images/trainer.png");
    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);

    int player_walking_animation[4][4] =
    {
        { 4, 5, 6, 7 },     // for trainer to move to the left,
        { 8, 9, 10, 11 },   // for trainer to move to the right,
        { 0, 1, 2, 3 },     // for trainer to move upwards,
        { 12, 13, 14, 15 }  // for trainer to move downwards
    };

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        1.0f,                      // width
        1.0f,                      // height
        PLAYER
    );
    
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(4.0f);
    
    /* Enemies' stuff */
    GLuint scatterbug_texture_id = Utility::load_texture("assets/images/scatterbug.png");
    GLuint froakie_texture_id = Utility::load_texture("assets/images/froakie.png");
    GLuint clodsire_texture_id = Utility::load_texture("assets/images/clodsire.png");
    
    // ————— SCATTERBUG ————— //
    int scatterbug_walking_animation[4][4] =
    {
        { 4, 5, 6, 7 },
        { 8, 9, 10, 11 },
        { 0, 1, 2, 3 },
        { 12, 13, 14, 15 }
    };
    
    m_game_state.enemies = new Entity[ENEMY_COUNT];
    
    m_game_state.enemies[0] = Entity(
        scatterbug_texture_id,          // texture id
        1.0f,                           // speed
        glm::vec3(0.0f, -9.81f, 0.0f),  // acceleration
        5.0f,                           // jumping power
        scatterbug_walking_animation,   // animation index sets
        0.0f,                           // animation time
        4,                              // animation frame amount
        0,                              // current animation index
        4,                              // animation column amount
        4,                              // animation row amount
        1.0f,                           // width
        1.0f,                           // height
        ENEMY
    );
    
    m_game_state.enemies[0].set_position(glm::vec3(10.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_ai_type(GUARD);
    
    // ————— FROAKIE ————— //
    int froakie_walking_animation[4][4] =
    {
        { 4, 5, 6, 7 },
        { 8, 9, 10, 11 },
        { 0, 1, 2, 3 },
        { 12, 13, 14, 15 }
    };
    
    m_game_state.enemies[1] = Entity(
                                     scatterbug_texture_id,             // texture id
        1.0f,                           // speed
        glm::vec3(0.0f, -9.81f, 0.0f),  // acceleration
        5.0f,                           // jumping power
        froakie_walking_animation,      // animation index sets
        0.0f,                           // animation time
        0,                              // animation frame amount
        0,                              // current animation index
        4,                              // animation column amount
        4,                              // animation row amount
        1.0f,                           // width
        1.0f,                           // height
        ENEMY
    );
    
    m_game_state.enemies[1].set_position(glm::vec3(11.0f, 0.0f, 0.0f));
    m_game_state.enemies[1].set_ai_type(GUARD);
    
    // ————— CLODSIRE ————— //
    int clodsire_walking_animation[4][4] =
    {
        { 4, 5, 6, 7 },
        { 8, 9, 10, 11 },
        { 0, 1, 2, 3 },
        { 12, 13, 14, 15 }
    };
    
//    m_game_state.enemies[2] = Entity(
//                                     scatterbug_texture_id,            // texture id
//        0.0f,                           // speed
//        glm::vec3(0.0f, -9.81f, 0.0f),  // acceleration
//        5.0f,                           // jumping power
//        clodsire_walking_animation,     // animation index sets
//        0.0f,                           // animation time
//        4,                              // animation frame amount
//        0,                              // current animation index
//        4,                              // animation column amount
//        4,                              // animation row amount
//        1.0f,                           // width
//        1.0f,                           // height
//        ENEMY
//    );
//    
//    m_game_state.enemies[2].set_position(glm::vec3(15.0f, 0.0f, 0.0f));
//    m_game_state.enemies[2].set_ai_type(ATTACK);

    /* BGM and SFX */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelA::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < 2; i++) {
        std::cout<< i << std::endl;
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
}

void LevelA::render(ShaderProgram *g_shader_program) {
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    m_game_state.enemies[0].render(g_shader_program);
    m_game_state.enemies[1].render(g_shader_program);
   // for (int i = 0; i < 2; i++) m_game_state.enemies[i].render(g_shader_program);
}
