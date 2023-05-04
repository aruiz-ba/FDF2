#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

//mesh.h

class Point {
public:
    Point(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}

    double getX() const { return m_x; }
    double getY() const { return m_y; }
    double getZ() const { return m_z; }

private:
    double m_x;
    double m_y;
    double m_z;
};

class Transform {
public:
    // Transform(double px, double py, double sz) : p_x(px), p_y(py), s_z(sz) {}
    Transform() : p_x(100), p_y(100), r_x(0), r_y(0), r_z(0), s_z(1), size(20) {}

    double getpX() const { return p_x; }
    double getpY() const { return p_y; }
    double getrX() const { return r_x; }
    double getrY() const { return r_y; }
    double getrZ() const { return r_z; }
    double getsZ() const { return s_z; }
    double getSize() const { return size; }

    void updateState(const Uint8* state){
      
      if (state[SDL_SCANCODE_A]) {
          p_x -= 4;
      }
      if (state[SDL_SCANCODE_D]) {
          p_x += 4;
      }
      if (state[SDL_SCANCODE_W]) {
          p_y -= 4;
      }
      if (state[SDL_SCANCODE_S]) {
          p_y += 4;
      }
      if (state[SDL_SCANCODE_P]) {
          size += 0.5;
      }
      if (state[SDL_SCANCODE_O]) {
          size -= 0.5;
      }

      if (state[SDL_SCANCODE_Z]) {
          s_z += 0.5;
      }

      if (state[SDL_SCANCODE_H]) {
         r_x = 2;
      }
      else if (state[SDL_SCANCODE_J]) {
          r_y = 2;
      }
      else if (state[SDL_SCANCODE_K]) {
          r_z = 2;
      }
      else {
        r_x = 0;
        r_y = 0;
        r_z = 0;
      }
      if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) {
        if (state[SDL_SCANCODE_H]) {
           r_x = -2;
        }
        else if (state[SDL_SCANCODE_J]) {
            r_y = -2;
        }
        else if (state[SDL_SCANCODE_K]) {
            r_z = -2;
        }
        else {
          r_x = 0;
          r_y = 0;
          r_z = 0;
        }
        if (state[SDL_SCANCODE_Z]) {
            s_z -= 0.5;
        }
      }
    }

private:
    //Position values
    double p_x;
    double p_y;

    //rotation switch
    double r_x;
    double r_y;
    double r_z;

    //Size values
    double s_z;
    double size;
};

class Mesh {
public:
    //Creation part
    Mesh(SDL_Renderer* renderer) : m_renderer(renderer) {}

    void addPoint(double x, double y, double z) {
        m_points.emplace_back(x, y, z);
    } 

    //Events
    void set_events()
    {
      const Uint8* state = SDL_GetKeyboardState(nullptr);
      tr.updateState(state);
      rotateX(tr.getrX());
      rotateY(tr.getrY());
      rotateZ(tr.getrZ());
    }

    //Transform part
    void rotateX(double angle) {
        double rad = angle * M_PI / 180.0;
        std::array<std::array<double, 4>, 4> mat = {{
            {1, 0, 0, 0},
            {0, std::cos(rad), std::sin(rad), 0},
            {0, -std::sin(rad), std::cos(rad), 0},
            {0, 0, 0, 1}
        }};
        transform(mat);
    }
    void rotateY(double angle) {
           double rad = angle * M_PI / 180.0;
           std::array<std::array<double, 4>, 4> mat = {{
               {std::cos(rad), 0, -std::sin(rad), 0},
               {0, 1, 0, 0},
               {std::sin(rad), 0, std::cos(rad), 0},
               {0, 0, 0, 1}
           }};
           transform(mat);
    }    
    void rotateZ(double angle) {
        double rad = angle * M_PI / 180.0;
        std::array<std::array<double, 4>, 4> mat = {{
            {std::cos(rad), std::sin(rad), 0, 0},
            {-std::sin(rad), std::cos(rad), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }};
        transform(mat);
    }

    //Drawing part
    void draw_rows(int row_size) const {
        if (m_points.size() < 2) {
            return;
        }

        // Set the draw color
        SDL_SetRenderDrawColor(m_renderer, 5, 255, 161, SDL_ALPHA_OPAQUE);

        // Draw lines between each point with increased separation
        for (size_t i = 1; i < m_points.size(); i++) 
        {
            double x1 = (m_points[i-1].getX() * tr.getSize()) + tr.getpX();
            double y1 = (m_points[i-1].getY() * tr.getSize()) + tr.getpY();
            double z1 = (m_points[i-1].getZ() * tr.getsZ());
            double x2 = (m_points[i].getX() * tr.getSize()) + tr.getpX();
            double y2 = (m_points[i].getY() * tr.getSize()) + tr.getpY();
            double z2 = (m_points[i].getZ() * tr.getsZ());
        
            // Check if i is the last point in the row
            if (i % row_size != 0) 
            {
                // Project 3D points onto 2D screen space
                int x1_proj = static_cast<int>(x1 + z1);
                int y1_proj = static_cast<int>(y1 + z1);
                int x2_proj = static_cast<int>(x2 + z2);
                int y2_proj = static_cast<int>(y2 + z2);
        
                SDL_RenderDrawLine(m_renderer, x1_proj, y1_proj, x2_proj, y2_proj);
            }
        }
    }
    void draw_columns(int col_size, int row_size) const {
        // Draw lines between each point with increased separation

        if (m_points.size() < 2) {
            return;
        }

        // Set the draw color
        SDL_SetRenderDrawColor(m_renderer, 5, 255, 161, SDL_ALPHA_OPAQUE);

        for (long unsigned int row = row_size; row < m_points.size(); row += row_size) 
        {
          for (int col = 0; col < col_size; col++) 
          {
            double x1 = (m_points[row + col - row_size].getX() * tr.getSize()) + tr.getpX();
            double y1 = (m_points[row + col - row_size].getY() * tr.getSize()) + tr.getpY();
            double z1 = (m_points[row + col - row_size].getZ() * tr.getsZ());
            double x2 = (m_points[row + col].getX() * tr.getSize()) + tr.getpX();
            double y2 = (m_points[row + col].getY() * tr.getSize()) + tr.getpY();
            double z2 = (m_points[row + col].getZ() * tr.getsZ());

            // if (row % row_size != 0) 
            // {
                // Project 3D points onto 2D screen space
                int x1_proj = static_cast<int>(x1 + z1);
                int y1_proj = static_cast<int>(y1 + z1);
                int x2_proj = static_cast<int>(x2 + z2);
                int y2_proj = static_cast<int>(y2 + z2);
        
                SDL_RenderDrawLine(m_renderer, x1_proj, y1_proj, x2_proj, y2_proj);
            // }
          }
        }
    }

    //Debbug
    void print() const 
    {
        for (const auto& point : m_points) 
        {
            std::cout << "(" << point.getX() << ", " << point.getY() << ", " << point.getZ() << ")" << std::endl;
        }
    }
private:
    std::vector<Point> m_points;
    SDL_Renderer* m_renderer;
    Transform tr;

    // Transform the points
    void transform(const std::array<std::array<double, 4>, 4>& mat) {
        for (auto& point : m_points) {
            double x = point.getX();
            double y = point.getY();
            double z = point.getZ();

            point = Point(
                mat[0][0]*x + mat[0][1]*y + mat[0][2]*z + mat[0][3],
                mat[1][0]*x + mat[1][1]*y + mat[1][2]*z + mat[1][3],
                mat[2][0]*x + mat[2][1]*y + mat[2][2]*z + mat[2][3]
            );
        }
    }
};

