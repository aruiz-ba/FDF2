#include "./mesh.h"

// main.cpp
// Define the Point and Mesh classes here...

void initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer) 
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << '\n';
        throw std::runtime_error("Failed to initialize SDL");
    }

    window = SDL_CreateWindow("Mesh Drawing Example",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << '\n';
        SDL_Quit();
        throw std::runtime_error("Failed to create window");
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) 
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to create renderer");
    }
}

void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer) 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawMesh(SDL_Renderer* renderer, Mesh& mesh, int row_size, int col_size) 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // mesh.rotateX(1);
    // mesh.rotateY(1);
    mesh.set_events();
    mesh.draw_rows(row_size);
    mesh.draw_columns(col_size, row_size);
    SDL_RenderPresent(renderer);
}

void handleEvents(bool& running) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            running = false;
        }
    }
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " input_file_path\n";
        return 1;
    }

    // Initialize SDL
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    try 
    {
        initializeSDL(window, renderer);
    }
    catch (std::runtime_error& e) 
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    // Open the input file
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: Could not open input file " << argv[1] << "\n";
        return 1;
    }

    // Read the input data
    std::vector<Point> points;
    std::string line;
    int col = 0;
    int row_size = 0;
    int col_size = 0;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        double z;
        int row = 0;
        while (iss >> z) {
            points.emplace_back(row, col, z);
            row++;
        }
        col++;
        row_size = row;
        col_size = row;
    }

    // Create a Mesh object and add the points to it
    Mesh mesh(renderer);
    for (const auto& point : points) 
    {
        mesh.addPoint(point.getX(), point.getY(), point.getZ());
    }

    // mesh.print();

    // Draw the mesh and wait for the user to close the window
    bool running = true;
    while (running) 
    {
        drawMesh(renderer, mesh, row_size, col_size);
        handleEvents(running);
    }

    // Clean up SDL resources
    cleanupSDL(window, renderer);

    return 0;
}
