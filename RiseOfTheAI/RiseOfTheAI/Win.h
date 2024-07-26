#include "Scene.h"

class Win : public Scene {
public:
    // ————— DESTRUCTOR ————— //
    ~Win();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
