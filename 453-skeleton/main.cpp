// yanisa srisa-ard
// 3015196

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <vector>
#include <glm/glm.hpp>
#include <cmath>
using namespace std;

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"

int recursionDepth = 0; 
bool recursionDepthChanged = true;

// type 1 = sierpinsky triangle
// type 2 = pythagoras tree
// type 3 = koch snowflake
// type 4 = dragon curve
// type 5 = hilbert curve
int type = 1;
bool fractalTypeChanged = true;

// colors for each recursion depth (10)
glm::vec3 depthColors[] = {
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 0.5f, 0.0f),
    glm::vec3(0.5f, 0.0f, 1.0f),
    glm::vec3(0.5f, 1.0f, 0.5f),
    glm::vec3(1.0f, 1.0f, 1.0f)
};

class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
		}

		// change to next type with the right key
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            recursionDepth = max(recursionDepth - 1, 0); // min depth = 0
            recursionDepthChanged = true;

			type += 1;
			if (type == 6) { type = 1; }

			cout << "Changed to: ";

			if (type == 1) { cout << "SIERPINSKYS TRIANGLE" << endl; }
			else if (type == 2) { cout << "PYTHAGORAS TREE" << endl; }
			else if (type == 3) { cout << "KOCH SNOWFLAKE" << endl;}
			else if (type == 4) { cout << "DRAGON CURVE" << endl; }
			else if (type == 5) { cout << "HILBERT CURVE" << endl; }

			fractalTypeChanged = true;
            recursionDepth = 0;
        }

		// go back to previous type with the left key
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            recursionDepth = max(recursionDepth - 1, 0); // min depth = 0
            recursionDepthChanged = true;

			type -= 1;
			if (type == 0) { type = 5; }

			cout << "Changed to: ";

			if (type == 1) { cout << "SIERPINSKYS TRIANGLE" << endl; }
			else if (type == 2) { cout << "PYTHAGORAS TREE" << endl; }
			else if (type == 3) { cout << "KOCH SNOWFLAKE" << endl;}
			else if (type == 4) { cout << "DRAGON CURVE" << endl; }
			else if (type == 5) { cout << "HILBERT CURVE" << endl; }

			fractalTypeChanged = true;
            recursionDepth = 0;
        }

		// increase recursion depth with up arrow
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            recursionDepth = min(recursionDepth + 1, 10); // max depth = 10
            recursionDepthChanged = true;
			cout << "Recursion depth increased to: " << recursionDepth << endl;
        }

        // decrease recursion depth with down arrow
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            recursionDepth = max(recursionDepth - 1, 0); // min depth = 0
            recursionDepthChanged = true;
			cout << "Recursion depth decreased to: " << recursionDepth << endl;
        }
	}

private:
	ShaderProgram& shader;
};

void generateSierpinskiTriangle(vector<glm::vec3>& verts, vector<glm::vec3>& cols, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int depth) {
	if (depth == 0) {
		// base case: push the triangle's vertices
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);

		// color based on current depth
		glm::vec3 color = depthColors[recursionDepth];
		cols.push_back(color);
		cols.push_back(color);
		cols.push_back(color);
	} else {
		// recursive case: add more triangles
		// find midpoints of each side of the triangle
		glm::vec3 mid1 = (p1 + p2) / 2.0f;
		glm::vec3 mid2 = (p2 + p3) / 2.0f;
		glm::vec3 mid3 = (p1 + p3) / 2.0f;

		// recursive case: (divide triangle into 3)
		generateSierpinskiTriangle(verts, cols, p1, mid1, mid3, depth - 1);
		generateSierpinskiTriangle(verts, cols, mid1, p2, mid2, depth - 1);
		generateSierpinskiTriangle(verts, cols, mid3, mid2, p3, depth - 1);
	}		
}

// doesn't work properly. at right square at the wrong angle :')
void generatePythagorasTree(vector<glm::vec3>& verts, vector<glm::vec3>& cols, glm::vec3 origin, float size, float angle, int depth) {
    // base square vertices
    glm::vec3 p1 = origin; // Bottom left
    glm::vec3 p2 = origin + glm::vec3(size * cos(angle), size * sin(angle), 0.0f); // bottom right
    glm::vec3 p3 = p2 + glm::vec3(size * cos(angle + M_PI / 2), size * sin(angle + M_PI / 2), 0.0f); // top right
    glm::vec3 p4 = origin + glm::vec3(size * cos(angle + M_PI / 2), size * sin(angle + M_PI / 2), 0.0f); // top left

    // add the base square vertices to the vector
    verts.push_back(p1);
    verts.push_back(p2);
    verts.push_back(p3);
    verts.push_back(p1);
    verts.push_back(p3);
    verts.push_back(p4);

    // color based on current depth
    glm::vec3 color = depthColors[recursionDepth - depth];
    for (int i = 0; i < 6; i++) {
        cols.push_back(color);
    }

    if (depth > 0) {
        // recursive case: draw two smaller squares at the top of the current square
        float newSize = size / sqrt(2); // scale down for the children squares

        // new origins for the left and right child squares
        glm::vec3 leftOrigin = p3; // align with the parent's top left vertex
        glm::vec3 rightOrigin = p4; // align with the parent's top right vertex

        // left and right squares
        generatePythagorasTree(verts, cols, leftOrigin, newSize, angle - M_PI / 4, depth - 1); // left square
        generatePythagorasTree(verts, cols, rightOrigin, newSize, angle + M_PI / 4, depth - 1); // right square
    }
}

void generateKochSnowflake(vector<glm::vec3>& verts, vector<glm::vec3>& cols, glm::vec3 p1, glm::vec3 p2, int depth) {
	if (depth == 0) {
        // base case: draw line segment
        verts.push_back(p1);
        verts.push_back(p2);

        // color based on current depth
        glm::vec3 color = depthColors[recursionDepth - depth];
        cols.push_back(color);
        cols.push_back(color);
    } else {
        // divide line segment into three equal parts
        glm::vec3 a = p1;
        glm::vec3 b = p1 + (p2 - p1) / 3.0f;
        glm::vec3 d = p1 + 2.0f * (p2 - p1) / 3.0f;
        glm::vec3 e = p2;

        glm::vec3 bd = d - b; // b->d vector

        float length = glm::length(bd);
        float height = length * sqrt(3.0f) / 2.0f;

        // b,d midpoint
        glm::vec3 mid = (b + d) / 2.0f;

        // normal direction (perpendicular to bd)
        glm::vec3 direction = glm::normalize(glm::vec3(-(d.y - b.y), d.x - b.x, 0.0f));

        // peak of equilateral triangle
        glm::vec3 peak = mid - direction * height;

        // recursive case: four new line segments
        generateKochSnowflake(verts, cols, a, b, depth - 1);
        generateKochSnowflake(verts, cols, b, peak, depth - 1);
        generateKochSnowflake(verts, cols, peak, d, depth - 1);
        generateKochSnowflake(verts, cols, d, e, depth - 1);
    }
}

void generateDragonCurve(vector<glm::vec3>& verts, vector<glm::vec3>& cols, glm::vec3 p1, glm::vec3 p2, int depth, bool clockwise) {
    if (depth == 0) {
        // base case: line segment
        verts.push_back(p1);
        verts.push_back(p2);

        // color based on current depth
        glm::vec3 color = depthColors[recursionDepth - depth];
        cols.push_back(color);
        cols.push_back(color);
    } else {
		//recursive case:
        // midpoint of line segment
        glm::vec3 mid = (p1 + p2) / 2.0f;

        // vector perpendicular to line segment
        glm::vec3 direction = glm::normalize(glm::vec3(-(p2.y - p1.y), p2.x - p1.x, 0.0f));
        
        // adjust midpoint based on direction (clockwise/counterclockwise)
        if (clockwise) {
            mid = mid - direction * glm::distance(p1, p2) / 2.0f;
        } else {
            mid = mid + direction * glm::distance(p1, p2) / 2.0f;
        }

        generateDragonCurve(verts, cols, p1, mid, depth - 1, true);  // clockwise
        generateDragonCurve(verts, cols, mid, p2, depth - 1, false); // counterclockwise
    }
}

// doesn't work properly:
//      when depth=0 it doesn't show up (probably cuz i did an instant return- but when i remove and change it, the program crashes ToT)
//      only works when depth is 1 (so need to increase depth for there to be a starting line segment)
//      wrong angles

void generateHilbertCurve(vector<glm::vec3>& verts, vector<glm::vec3>& cols, glm::vec3 p, float size, int depth, int orientation) {
    float halfSize = size / 2.0f;

    glm::vec3 p1, p2, p3, p4;

    if (depth == 0) {
        return;
    }

    // draw four subcurves in different orders -- depending on orientation
    switch (orientation) {
        case 0: // rotate left
            p1 = p + glm::vec3(0, 0, 0);
            p2 = p + glm::vec3(0, halfSize, 0);
            p3 = p + glm::vec3(halfSize, halfSize, 0);
            p4 = p + glm::vec3(halfSize, 0, 0);
            break;
        case 1: // rotate up
            p1 = p + glm::vec3(0, halfSize, 0);
            p2 = p + glm::vec3(0, 0, 0);
            p3 = p + glm::vec3(halfSize, 0, 0);
            p4 = p + glm::vec3(halfSize, halfSize, 0);
            break;
        case 2: // rotate right
            p1 = p + glm::vec3(halfSize, halfSize, 0);
            p2 = p + glm::vec3(halfSize, 0, 0);
            p3 = p + glm::vec3(0, 0, 0);
            p4 = p + glm::vec3(0, halfSize, 0);
            break;
        case 3: // rotate down
            p1 = p + glm::vec3(halfSize, 0, 0);
            p2 = p + glm::vec3(halfSize, halfSize, 0);
            p3 = p + glm::vec3(0, halfSize, 0);
            p4 = p + glm::vec3(0, 0, 0);
            break;
    }

    // recursive case: generate smaller hilbert curves
    generateHilbertCurve(verts, cols, p1, halfSize, depth - 1, (orientation + 3) % 4); // rotate left
    verts.push_back(p1);
    verts.push_back(p2);
    generateHilbertCurve(verts, cols, p2, halfSize, depth - 1, orientation);           // no rotation
    verts.push_back(p2);
    verts.push_back(p3);
    generateHilbertCurve(verts, cols, p3, halfSize, depth - 1, orientation);           // no rotation
    verts.push_back(p3);
    verts.push_back(p4);
    generateHilbertCurve(verts, cols, p4, halfSize, depth - 1, (orientation + 1) % 4); // rotate right

	// color based on current depth
    glm::vec3 color = depthColors[recursionDepth - depth];
    for (int i = 0; i < verts.size(); i++) {
        cols.push_back(color);
    }
}

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	window.setCallbacks(make_shared<MyCallbacks>(shader)); // can also update callbacks to new ones

	// GEOMETRY
	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> cols;

	// initial vertices for an equilateral triangle
    glm::vec3 p1(-0.5f, -0.5f, 0.f);
    glm::vec3 p2(0.5f, -0.5f, 0.f);
    glm::vec3 p3(0.0f, 0.5f, 0.f);

	// generate initial fractal-- by default
	generateSierpinskiTriangle(verts, cols, p1, p2, p3, recursionDepth);

    // set vertices and colors to the GPU
    cpuGeom.verts = verts;
    cpuGeom.cols = cols;
    gpuGeom.setVerts(cpuGeom.verts);
    gpuGeom.setCols(cpuGeom.cols);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		if (recursionDepthChanged || fractalTypeChanged) {
			verts.clear();
			cols.clear();
			
            // sierpinsky triangle
			if (type == 1) { generateSierpinskiTriangle(verts, cols, p1, p2, p3, recursionDepth); }
			
            // pythagoras tree
			if (type == 2) { 
				glm::vec3 baseOrigin(-0.5f, -0.5f, 0.f);
				float baseSize = 0.5f;
				float baseAngle = 0.0f;
				generatePythagorasTree(verts, cols, baseOrigin, baseSize, baseAngle, recursionDepth);
			}

            // koch snowflake
			if (type == 3) { 
				glm::vec3 p1(-0.5f, -0.3f, 0.f);
    			glm::vec3 p2(0.5f, -0.3f, 0.f);
				glm::vec3 p3(0.0f, 0.5f, 0.f);

				generateKochSnowflake(verts, cols, p1, p2, recursionDepth);
				generateKochSnowflake(verts, cols, p2, p3, recursionDepth);
				generateKochSnowflake(verts, cols, p3, p1, recursionDepth);
			}

            // dragon curve
			if (type == 4) { 
				glm::vec3 p1(-0.5f, 0.0f, 0.0f);
				glm::vec3 p2(0.5f, 0.0f, 0.0f);
				generateDragonCurve(verts, cols, p1, p2, recursionDepth, true);
			}

            // hilbert curve
			if (type == 5) {
				glm::vec3 p(-0.5f, -0.5f, 0.0f);
				float size = 1.0f;
				int orientation = 0;
				generateHilbertCurve(verts, cols, p, size, recursionDepth, orientation);
			}

			cpuGeom.verts = verts;
			cpuGeom.cols = cols;
			gpuGeom.setVerts(cpuGeom.verts);
			gpuGeom.setCols(cpuGeom.cols);

			// update CPU and GPU geometries
			cpuGeom.verts = verts;
			cpuGeom.cols = cols;
			gpuGeom.setVerts(cpuGeom.verts);
			gpuGeom.setCols(cpuGeom.cols);

			recursionDepthChanged = false;
			fractalTypeChanged = false;
		}

		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		if ((type == 3) || (type == 4) || (type == 5)) {
    		glLineWidth(2.0f);
			glDrawArrays(GL_LINES, 0, verts.size());
		} else {
			glDrawArrays(GL_TRIANGLES, 0, verts.size());
		}

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
