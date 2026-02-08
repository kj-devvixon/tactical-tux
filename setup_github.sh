#!/bin/bash

echo "🐧 Tactical Tux - GitHub Setup Script"
echo "======================================"
echo ""

# Check if git is initialized
if [ ! -d .git ]; then
    echo "Initializing git repository..."
    git init
fi

# Add all files
echo "Adding files to git..."
git add .

# Create initial commit
echo "Creating initial commit..."
git commit -m "🎮 Initial commit - Tactical Tux Early Alpha

Features:
- First-person camera system
- WASD movement controls
- Physics and collision detection
- Crosshair overlay
- Map editor
- Basic 3D rendering with OpenGL
- Custom shader system

This is an early alpha release. Expect bugs and missing features!"

echo ""
echo "✅ Git repository initialized!"
echo ""
echo "Next steps:"
echo "1. Create a new repository on GitHub: https://github.com/new"
echo "2. Run these commands:"
echo ""
echo "   git remote add origin https://github.com/YOUR_USERNAME/tactical-tux.git"
echo "   git branch -M main"
echo "   git push -u origin main"
echo ""
echo "3. Add your screenshot to screenshots/ folder"
echo "4. Update README.md with your GitHub username"
echo ""
echo "Good luck! 🚀"
