#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const int GRID_SIZE = 200; 
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;
const float DOMAIN_HEIGHT = 1.0f; 
const float DT = 1.0f / 60.0f;
const int NUM_ITERATIONS = 30;
const float DENSITY = 1000.0f;     
const float GRAVITY = -1.0f;
const float OVER_RELAXATION = 1.5f;


const int U_FIELD = 0;
const int V_FIELD = 1;
const int S_FIELD = 2;

class Fluid {
private:
    int numX, numY, numCells;
    float h;
    std::vector<float> u, v;
    std::vector<float> newU, newV;
    std::vector<float> p;
    std::vector<float> s;
    std::vector<float> m;
    std::vector<float> newM;

public:
    Fluid(int numX, int numY, float h) : numX(numX + 2), numY(numY + 2), h(h) {
        numCells = this->numX * this->numY;
    
        u.resize(numCells, 0.0f);
        v.resize(numCells, 0.0f);
        newU.resize(numCells, 0.0f);
        newV.resize(numCells, 0.0f);
        p.resize(numCells, 0.0f);
        s.resize(numCells, 1.0f); 
        m.resize(numCells, 0.0f);
        newM.resize(numCells, 0.0f);
        
        setupCircularObstacle();
    }
    
    void setupCircularObstacle() {
        float centerX = numX * h * 0.5f;
        float centerY = numY * h * 0.5f;
        float radius = 0.08f; 
        
        for (int i = 0; i < numX; i++) {
            for (int j = 0; j < numY; j++) {
                float x = (i + 0.5f) * h;
                float y = (j + 0.5f) * h;
                
                float dx = x - centerX;
                float dy = y - centerY;
                float distance = sqrt(dx*dx + dy*dy);
                
                if (distance < radius) {
                    s[i * numY + j] = 0.0f; 
                    m[i * numY + j] = 0.0f;  
                }
            }
        }
    }
    
    void integrate(float dt, float gravity) {
        for (int i = 1; i < numX; i++) {
            for (int j = 1; j < numY - 1; j++) {
                if (s[i * numY + j] != 0.0f && s[i * numY + j - 1] != 0.0f) {
                    v[i * numY + j] += gravity * dt;
                }
            }
        }
    }
    
    void solveIncompressibility(int numIters, float dt) {
        float cp = DENSITY * h / dt;
        
        for (int iter = 0; iter < numIters; iter++) {
            for (int i = 1; i < numX - 1; i++) {
                for (int j = 1; j < numY - 1; j++) {
                    if (s[i * numY + j] == 0.0f) continue;
                    
                    float sx0 = s[(i - 1) * numY + j];
                    float sx1 = s[(i + 1) * numY + j];
                    float sy0 = s[i * numY + j - 1];
                    float sy1 = s[i * numY + j + 1];
                    float s = sx0 + sx1 + sy0 + sy1;
                    
                    if (s == 0.0f) continue;
                    
                    float div = u[(i + 1) * numY + j] - u[i * numY + j] +
                               v[i * numY + j + 1] - v[i * numY + j];
                    
                    float pressure = -div / s;
                    pressure *= OVER_RELAXATION;
                    p[i * numY + j] += cp * pressure;
                    
                    u[i * numY + j] -= sx0 * pressure;
                    u[(i + 1) * numY + j] += sx1 * pressure;
                    v[i * numY + j] -= sy0 * pressure;
                    v[i * numY + j + 1] += sy1 * pressure;
                }
            }
        }
    }
    
    
    void extrapolate() {
        
        for (int i = 0; i < numX; i++) {
            u[i * numY + 0] = u[i * numY + 1];
            u[i * numY + numY - 1] = u[i * numY + numY - 2];
        }
        
        for (int j = 0; j < numY; j++) {
            v[0 * numY + j] = v[1 * numY + j];
            v[(numX - 1) * numY + j] = v[(numX - 2) * numY + j];
        }
                for (int i = 0; i < numX; i++) {
            for (int j = 0; j < numY; j++) {
                if (i == 0 || i == numX - 1 || j == 0 || j == numY - 1) {
                    u[i * numY + j] *= 0.9f; 
                    v[i * numY + j] *= 0.9f;
                }
            }
        }
    }
    
    
    float sampleField(float x, float y, int field) {
        float h1 = 1.0f / h;
        float h2 = 0.5f * h;
        
        x = std::max(std::min(x, numX * h), h);
        y = std::max(std::min(y, numY * h), h);
        
        float dx = 0.0f, dy = 0.0f;
        std::vector<float>* f;
        
        switch (field) {
            case U_FIELD: f = &u; dy = h2; break;
            case V_FIELD: f = &v; dx = h2; break;
            case S_FIELD: f = &m; dx = h2; dy = h2; break;
            default: return 0.0f;
        }
        
        int x0 = std::min((int)std::floor((x - dx) * h1), numX - 1);
        float tx = ((x - dx) - x0 * h) * h1;
        int x1 = std::min(x0 + 1, numX - 1);
        
        int y0 = std::min((int)std::floor((y - dy) * h1), numY - 1);
        float ty = ((y - dy) - y0 * h) * h1;
        int y1 = std::min(y0 + 1, numY - 1);
        
        float sx = 1.0f - tx;
        float sy = 1.0f - ty;
        
        return sx * sy * (*f)[x0 * numY + y0] +
               tx * sy * (*f)[x1 * numY + y0] +
               tx * ty * (*f)[x1 * numY + y1] +
               sx * ty * (*f)[x0 * numY + y1];
    }
    
    
    float avgU(int i, int j) {
        return (u[i * numY + j - 1] + u[i * numY + j] +
                u[(i + 1) * numY + j - 1] + u[(i + 1) * numY + j]) * 0.25f;
    }
    
    float avgV(int i, int j) {
        return (v[(i - 1) * numY + j] + v[i * numY + j] +
                v[(i - 1) * numY + j + 1] + v[i * numY + j + 1]) * 0.25f;
    }
    
    
    void advectVel(float dt) {
        newU = u;
        newV = v;
        
        float h2 = 0.5f * h;
        
        for (int i = 1; i < numX; i++) {
            for (int j = 1; j < numY; j++) {
                
                if (s[i * numY + j] != 0.0f && s[(i - 1) * numY + j] != 0.0f && j < numY - 1) {
                    float x = i * h;
                    float y = j * h + h2;
                    float u_comp = u[i * numY + j];
                    float v_comp = avgV(i, j);
                    
                    x = x - dt * u_comp;
                    y = y - dt * v_comp;
                    u_comp = sampleField(x, y, U_FIELD);
                    newU[i * numY + j] = u_comp;
                }
                
                
                if (s[i * numY + j] != 0.0f && s[i * numY + j - 1] != 0.0f && i < numX - 1) {
                    float x = i * h + h2;
                    float y = j * h;
                    float u_comp = avgU(i, j);
                    float v_comp = v[i * numY + j];
                    
                    x = x - dt * u_comp;
                    y = y - dt * v_comp;
                    v_comp = sampleField(x, y, V_FIELD);
                    newV[i * numY + j] = v_comp;
                }
            }
        }
        
        u = newU;
        v = newV;
    }
    
    
    void advectSmoke(float dt) {
        newM = m;
        
        float h2 = 0.5f * h;
        
        for (int i = 1; i < numX - 1; i++) {
            for (int j = 1; j < numY - 1; j++) {
                if (s[i * numY + j] != 0.0f) {
                    float u_comp = (u[i * numY + j] + u[(i + 1) * numY + j]) * 0.5f;
                    float v_comp = (v[i * numY + j] + v[i * numY + j + 1]) * 0.5f;
                    float x = i * h + h2 - dt * u_comp;
                    float y = j * h + h2 - dt * v_comp;
                    
                    newM[i * numY + j] = sampleField(x, y, S_FIELD);
                }
            }
        }
        m = newM;
    }
    
    
    void simulate(float dt, float gravity, int numIters) {
        integrate(dt, gravity);
        
        std::fill(p.begin(), p.end(), 0.0f);
        solveIncompressibility(numIters, dt);
        
        extrapolate();
        advectVel(dt);
        advectSmoke(dt);
        
        
        fadeDensity();
    }
    
    
    void fadeDensity() {
        for (int i = 0; i < numCells; i++) {
            m[i] *= 0.998f;  
        }
    }
    
    
    void addDensity(int x, int y, float amount) {
        if (x >= 0 && x < numX && y >= 0 && y < numY) {
            m[x * numY + y] += amount;
        }
    }
    
    void addVelocity(int x, int y, float amountX, float amountY) {
        if (x >= 0 && x < numX && y >= 0 && y < numY) {
            u[x * numY + y] += amountX;
            v[x * numY + y] += amountY;
        }
    }
    
    
    const std::vector<float>& getDensity() const { return m; }
    const std::vector<float>& getPressure() const { return p; }
    const std::vector<float>& getSolid() const { return s; }
    int getNumX() const { return numX; }
    int getNumY() const { return numY; }
};


Fluid* fluid = nullptr;
bool mouseDown = false;
float mouseX = 0.0f, mouseY = 0.0f;
float lastMouseX = 0.0f, lastMouseY = 0.0f;
bool showPressure = false;
bool showSmoke = true;


void handleMouseInput(float x, float y, bool isDown) {
    mouseX = x;
    mouseY = y;
    
    if (isDown && !mouseDown) {
        mouseDown = true;
    } else if (!isDown && mouseDown) {
        mouseDown = false;
    }
    
    if (mouseDown && fluid) {
        float simX = x * fluid->getNumX();
        float simY = (1.0f - y) * fluid->getNumY(); 
        
        int centerX = (int)simX;
        int centerY = (int)simY;
        int radius = 8;  
        
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                int px = centerX + dx;
                int py = centerY + dy;
                
                float distance = sqrt(dx*dx + dy*dy);
                if (distance <= radius) {
                    float falloff = 1.0f - (distance / radius);
                    fluid->addDensity(px, py, 8.0f * falloff);  
                }
            }
        }
        
        float velX = (x - lastMouseX) * 15.0f;
        float velY = (lastMouseY - y) * 15.0f;  
        
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int px = centerX + dx;
                int py = centerY + dy;
                fluid->addVelocity(px, py, velX, velY);
            }
        }
    }
    
    lastMouseX = x;
    lastMouseY = y;
}


void render() {
    if (!fluid) return;
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    const std::vector<float>& data = showPressure ? fluid->getPressure() : fluid->getDensity();
    int numX = fluid->getNumX();
    int numY = fluid->getNumY();
    
    float minVal = *std::min_element(data.begin(), data.end());
    float maxVal = *std::max_element(data.begin(), data.end());
    if (maxVal - minVal < 1e-6f) maxVal = minVal + 1.0f;
    
    float cellWidth = 2.0f / numX;
    float cellHeight = 2.0f / numY;
    
        for (int i = 0; i < numX; i++) {
        for (int j = 0; j < numY; j++) {
            if (fluid->getSolid()[i * numY + j] == 0.0f) {
               
                glColor3f(0.2f, 0.2f, 0.4f);
            } else {
                float value = data[i * numY + j];
                float normalized = (value - minVal) / (maxVal - minVal);
                
                    if (showPressure) {
                    
                    if (normalized < 0.33f) {
                        float t = normalized / 0.33f;
                        glColor3f(0.0f, t, 1.0f);  
                    } else if (normalized < 0.66f) {
                        float t = (normalized - 0.33f) / 0.33f;
                        glColor3f(0.0f, 1.0f, 1.0f - t);  
                    } else {
                        float t = (normalized - 0.66f) / 0.34f;
                        glColor3f(t, 1.0f, 0.0f);  
                    }
                } else {
                    
                    if (normalized < 0.5f) {
                        float t = normalized / 0.5f;
                        glColor3f(0.0f, 0.3f + 0.4f * t, 0.8f + 0.2f * t);  
                    } else {
                        float t = (normalized - 0.5f) / 0.5f;
                        glColor3f(0.4f * t, 0.7f + 0.3f * t, 1.0f);  
                    }
                }
            }
            
            float x = -1.0f + i * cellWidth;
            float y = -1.0f + j * cellHeight;
            
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + cellWidth, y);
            glVertex2f(x + cellWidth, y + cellHeight);
            glVertex2f(x, y + cellHeight);
            glEnd();
        }
    }
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        float normalizedX = x / WINDOW_WIDTH;
        float normalizedY = y / WINDOW_HEIGHT;
        handleMouseInput(normalizedX, normalizedY, action == GLFW_PRESS);
    }
}

void mouseMoveCallback(GLFWwindow* window, double x, double y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float normalizedX = x / WINDOW_WIDTH;
        float normalizedY = y / WINDOW_HEIGHT;
        handleMouseInput(normalizedX, normalizedY, true);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_P:
                showPressure = !showPressure;
                showSmoke = !showPressure;
                break;
            case GLFW_KEY_S:
                showSmoke = !showSmoke;
                showPressure = !showSmoke;
                break;
            case GLFW_KEY_R:
                
                delete fluid;
                fluid = new Fluid(GRID_SIZE, GRID_SIZE, DOMAIN_HEIGHT / GRID_SIZE);
                break;
        }
    }
}

int main() {
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fluid Simulation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    
    fluid = new Fluid(GRID_SIZE, GRID_SIZE, DOMAIN_HEIGHT / GRID_SIZE);
    
    
    while (!glfwWindowShouldClose(window)) {
        
        fluid->simulate(DT, GRAVITY, NUM_ITERATIONS);
        
        
        render();
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete fluid;
    glfwTerminate();
    
    return 0;
}