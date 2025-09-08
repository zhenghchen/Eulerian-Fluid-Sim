# 🌊 High-Resolution Fluid Simulation

A stunning real-time fluid simulation built in C++ with OpenGL, featuring beautiful water-like physics, interactive mouse controls, and collision detection with obstacles.

![Fluid Simulation](https://img.shields.io/badge/Status-Ready%20to%20Use-brightgreen)
![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-orange)
![License](https://img.shields.io/badge/License-MIT-green)

## ✨ Features

### 🎨 **Visual Excellence**
- **High Resolution**: 200x200 grid for smooth, detailed fluid behavior
- **Beautiful Colors**: Water-like blue-to-white gradients with scientific pressure visualization
- **Black Background**: Professional dark theme for maximum visual impact
- **Real-time Rendering**: 60+ FPS with modern OpenGL

### 🌊 **Advanced Physics**
- **Eulerian Fluid Dynamics**: Proper incompressible Navier-Stokes equations
- **Pressure Projection**: Divergence-free velocity fields for realistic flow
- **Semi-Lagrangian Advection**: Stable numerical methods for smooth simulation
- **Collision Detection**: Interactive circular obstacle for flow visualization

### 🎮 **Interactive Controls**
- **Mouse Interaction**: Click and drag to create fluid streams
- **Volume Generation**: Large 17x17 area fluid generation for impressive effects
- **Real-time Response**: Immediate fluid manipulation with natural flow patterns
- **Multiple Views**: Toggle between density and pressure visualization

## 🚀 Quick Start

### Prerequisites

#### macOS (using Homebrew)
```bash
brew install glfw glew glm cmake
```

#### Ubuntu/Debian
```bash
sudo apt-get install libglfw3-dev libglew-dev libglm-dev cmake build-essential
```

#### Windows
- Install Visual Studio with C++ support
- Install vcpkg and use it to install glfw, glew, and glm

### Building & Running

```bash
# Clone the repository
git clone <your-repo-url>
cd Fluid\ Simulator

# Build using the automated script
./build.sh

# Run the simulation
cd build && ./fluid_simulation
```

### Alternative Build Methods

```bash
# Using CMake directly
mkdir build && cd build
cmake ..
make

# Using Makefile
make
```

## 🎯 Controls

| Key/Mouse | Action |
|-----------|--------|
| **Left Mouse + Drag** | Create fluid streams and flow patterns |
| **P Key** | Toggle pressure visualization (blue→cyan→yellow→red) |
| **S Key** | Toggle smoke/density visualization (blue→white) |
| **R Key** | Reset the simulation |
| **ESC** | Exit the application |

## 🔬 Technical Details

### Physics Implementation
- **Grid Resolution**: 200x200 cells for high detail
- **Time Step**: 1/60 seconds for stability
- **Iterations**: 30 pressure projection steps
- **Gravity**: -1.0 for water-like behavior
- **Over-relaxation**: 1.5x for faster convergence

### Rendering Pipeline
- **OpenGL 2.1+**: Compatible with most systems
- **Legacy Rendering**: Uses GL_QUADS for reliable performance
- **Color Mapping**: Scientific visualization with smooth gradients
- **Real-time Updates**: 60+ FPS on modern hardware

### Performance
- **Memory Usage**: ~2MB for 200x200 grid
- **CPU Optimized**: Efficient algorithms for real-time simulation
- **GPU Accelerated**: OpenGL rendering for smooth visuals

## 🎨 Visual Features

### Fluid Visualization
- **Density Mode**: Beautiful blue-to-white water-like colors
- **Pressure Mode**: Scientific blue→cyan→yellow→red mapping
- **Smooth Gradients**: Professional color interpolation
- **High Contrast**: Black background for maximum visual impact

### Collision System
- **Circular Obstacle**: Interactive collision detection
- **Flow Patterns**: Realistic fluid behavior around obstacles
- **Vortex Formation**: Natural wake effects and turbulence
- **Pressure Buildup**: Visual pressure gradients around obstacles

## 🛠️ Customization

### Adjusting Fluid Behavior
Edit the constants in `fluid_sim.cpp`:

```cpp
const int GRID_SIZE = 200;        // Resolution (higher = more detail)
const float GRAVITY = -1.0f;      // Gravity strength
const float DT = 1.0f / 60.0f;    // Time step
const int NUM_ITERATIONS = 30;    // Pressure solver iterations
```

### Modifying Visuals
```cpp
// In the color mapping section
glColor3f(r, g, b);  // Adjust RGB values for different colors
```

### Adding Obstacles
```cpp
// In setupCircularObstacle()
float radius = 0.08f;  // Obstacle size
float centerX = numX * h * 0.5f;  // X position
float centerY = numY * h * 0.5f;  // Y position
```

## 📁 Project Structure

```
Fluid Simulator/
├── fluid_sim.cpp          # Main simulation code
├── CMakeLists.txt         # CMake build configuration
├── Makefile              # Alternative build system
├── build.sh              # Automated build script
├── README.md             # This file
└── build/                # Build output directory
    └── fluid_simulation  # Executable
```

## 🎓 Educational Value

This simulation demonstrates:
- **Computational Fluid Dynamics**: Real-world physics simulation
- **Numerical Methods**: Pressure projection and advection schemes
- **Computer Graphics**: Real-time OpenGL rendering
- **Interactive Systems**: Mouse-based user interaction
- **Scientific Visualization**: Color mapping for data representation

## 🔧 Troubleshooting

### Build Issues
- **Missing libraries**: Ensure all dependencies are installed
- **OpenGL errors**: Check graphics driver compatibility
- **Compilation errors**: Verify C++17 support

### Runtime Issues
- **Low FPS**: Reduce GRID_SIZE or NUM_ITERATIONS
- **No fluid generation**: Check mouse coordinate mapping
- **Visual artifacts**: Verify OpenGL context initialization

## 📊 Performance Tips

- **Higher resolution**: Increase GRID_SIZE for more detail (requires more CPU)
- **Smoother simulation**: Increase NUM_ITERATIONS (slower but more accurate)
- **Faster rendering**: Reduce fluid generation radius for better performance
- **Visual quality**: Adjust fade rate for longer-lasting effects

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- GPU acceleration with compute shaders
- Multiple obstacle types
- Different fluid viscosities
- Advanced visualization modes
- Performance optimizations

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🙏 Acknowledgments

- Based on the excellent fluid simulation algorithms from the HTML/JavaScript examples
- Uses modern C++ and OpenGL for high-performance rendering
- Inspired by real-world computational fluid dynamics research

---

**Ready to create stunning fluid simulations? Start building and watch the magic happen! 🌊✨**
