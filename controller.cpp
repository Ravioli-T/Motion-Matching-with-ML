#include <raylib.h>
#include "character.h"
#include "bvh_loader.h"

Mesh make_character_mesh(character& c)
{
    Mesh mesh = { 0 };

    mesh.vertexCount = c.positions.size;
    mesh.triangleCount = c.triangles.size / 3;
    mesh.vertices = (float*)MemAlloc(c.positions.size * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(c.texcoords.size * 2 * sizeof(float));
    mesh.normals = (float*)MemAlloc(c.normals.size * 3 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(c.triangles.size * sizeof(unsigned short));

    memcpy(mesh.vertices, c.positions.data, c.positions.size * 3 * sizeof(float));
    memcpy(mesh.texcoords, c.texcoords.data, c.texcoords.size * 2 * sizeof(float));
    memcpy(mesh.normals, c.normals.data, c.normals.size * 3 * sizeof(float));
    memcpy(mesh.indices, c.triangles.data, c.triangles.size * sizeof(unsigned short));

    UploadMesh(&mesh, true);

    return mesh;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib 3D");

    // Define the 3d camera
    Camera camera = { 0 };
    camera.position.x = 1.5f;
    camera.position.y = 1.5f;
    camera.position.z = 1.5f; // Camera position
    camera.target.x = 0.0f;
    camera.target.y = 0.4f;
    camera.target.z = 0.0f; // Camera looking at point
    camera.up.x = 0.0f;
    camera.up.y = 1.0f;
    camera.up.z = 0.0f; // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Character
    character character_data;
    character_load(character_data, "./resources/character.bin");

    Shader character_shader = LoadShader("./resources/character.vs", "./resources/character.fs");
    Mesh character_mesh = make_character_mesh(character_data);
    Model character_model = LoadModelFromMesh(character_mesh);
    character_model.materials[0].shader = character_shader;

    Vector3 character_position = { 0.0f, 0.0f, 0.0f };  // Character position in the world
    float character_speed = 2.0f; // Character movement speed

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Movement logic
        if (IsKeyDown(KEY_W)) character_position.z -= character_speed * GetFrameTime();
        if (IsKeyDown(KEY_S)) character_position.z += character_speed * GetFrameTime();
        if (IsKeyDown(KEY_A)) character_position.x -= character_speed * GetFrameTime();
        if (IsKeyDown(KEY_D)) character_position.x += character_speed * GetFrameTime();

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawModel(character_model, character_position, 1.0f, WHITE); // Draw character model

        DrawGrid(10, 1.0f);         // Draw a grid

        EndMode3D();

        DrawText("First Person Camera and Character Rendering Example", 10, GetScreenHeight() - 20, 20, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(character_model);    // Unload character model
    UnloadShader(character_shader);  // Unload character shader

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}