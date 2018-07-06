#include<iostream>
#include<vector>
#include <math.h>

class Vector2 { public: float x,y; };
class Vector3 { 
	public: 
	float x,y,z;
	Vector3 operator + (Vector3 v) const
    { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator - (Vector3 v) const
    { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator * (float f) const
    { return Vector3(x*f,y*f,z*f); }
    Vector3 operator - () const
    { return Vector3(-x,-y,-z); }
    Vector3 operator / (float f) const
    { return Vector3(x/f,y/f,z/f); }
	Vector3(float a, float b,float c):x(a),y(b),z(c){}
	Vector3(){}
	Vector3 direction(){
	return Vector3(x/sqrt(x*x+y*y+z*z),y/sqrt(x*x+y*y+z*z),z/sqrt(x*x+y*y+z*z));
					}
	Vector3 cross(Vector3 v){
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	float dot(Vector3 v){
		return (x * v.x + y * v.y + z * v.z);
	}
	float length(){
		return (sqrt(x*x+y*y+z*z));
	}
	static Vector3 unitY(){return Vector3(0.0f,1.0f,0.0f);}
	static Vector3 unitX(){return Vector3(1.0f,0.0f,0.0f);}
	static Vector3 unitZ(){return Vector3(0.0f,0.0f,1.0f);}
 };
typedef Vector2 Point2;
typedef Vector3 Point3;
class Color3 { 
	public: float r,g,b; 
			Color3(){}
			Color3(float a, float b, float c):r(a),g(b),b(c){}
			Color3(Vector3 v):r(v.x),g(v.y),b(v.z){}
			Color3(const Color3& c):r(c.r),g(c.g),b(c.b){}
			Color3 operator / (float f) const{return Color3(r/f,g/f,b/f);}
			Color3 operator * (Color3 c) const{return(Color3(r*c.r, g*c.g, b*c.b));}
			Color3 operator * (float f) const{return Color3(r*f,g*f,b*f);}
			Color3 operator += (Color3 c){return Color3(r+=(c.r), g+=(c.g), b+=(c.b));}
			Color3 operator + (Color3 c) const{return Color3(r+c.r,g+c.g,b+c.b);}
			static Color3 red(){return Color3(1.0f,0.0f,0.0f);}
			static Color3 green(){return Color3(0.0f,1.0f,0.0f);}
			static Color3 blue(){return Color3(0.0f,0.0f,1.0f);}
			static Color3 white(){return Color3(1.0f,1.0f,1.0f);}
			static Color3 indigo(){return Color3(0.05f,0.31f,0.55f);}
			static Color3 yellow(){return Color3(0.93f,0.93f,0.0f);}
			static Color3 violet(){return Color3(0.93,0.51f,0.93f);}
			static Color3 orange(){return Color3(0.8f,0.2f,0.2f);}
			static Color3 black(){return Color3(0.0f,0.0f,0.0f);}
};
// class Radiance3 Color3;
// class Power3 Color3;
typedef Color3 Radiance3;
typedef Color3 Power3;

class Ray{
	private: 
		Point3 m_origin;
		Vector3 m_direction;
	public: 
		Ray(Point3 org, Vector3 dir): m_origin(org),m_direction(dir){}

		Point3 origin() const {return m_origin;}
		Vector3 direction() const {return m_direction;}
};

class Image{

private: 
	int m_width;
	int m_height;
	std::vector<Radiance3> m_data;	

	int PPMGammaEncode(float radiance,float displayConstant) const;

public:
	Image(int width,int height): m_width(width),m_height(height),m_data(width*height){}

	int width() const{return m_width;}
	int height() const{return m_height;}

	void set(int x,int y,Radiance3 value){
		m_data[x+y*m_width] = value;
	}

	const Radiance3& get(int x,int y) const{
		return m_data[x+y*m_width];
	}

	void save(const std::string& filename,float displayConstant = 15.0f) const;

};

int Image::PPMGammaEncode(float radiance, float d) const{
	return int(pow(std::min(1.0f,std::max(0.0f,radiance*d)),1.0f/2.2f)*255.0f);
}

void Image::save(const std::string& filename,float d) const{
	FILE* file = fopen(filename.c_str(),"wt");
	fprintf(file,"P3 %d %d 255\n",m_width,m_height);
	for(int y = 0;y<m_height;++y){
		fprintf(file, "\n# y = %d\n", y);
		for(int x = 0;x<m_width;++x){
			const Radiance3& c(get(x,y));
			fprintf(file, "%d %d %d\n", PPMGammaEncode(c.r, d)
									  , PPMGammaEncode(c.g, d)
									  , PPMGammaEncode(c.b, d));
		}
	}
	fclose(file);
}

class Triangle{
private: 
	Point3 m_vertex[3];
	Vector3 m_normal[3];
	BSDF m_bsdf;

public:
	Triangle( Vector3 v0, Vector3 v1, Vector3 v2,
              Vector3 n0, Vector3 n1, Vector3 n2, BSDF bsdf): m_bsdf(bsdf){
        m_vertex[0] = v0;  m_normal[0] = n0;
        m_vertex[1] = v1;  m_normal[1] = n1;
        m_vertex[2] = v2;  m_normal[2] = n2;
    }
	Point3 vertex(int i) const{return m_vertex[i];}
	Vector3 normal(int i) const{return m_normal[i];}
	BSDF bsdf() const{return m_bsdf;}

};

class Light{
public:
	Point3 position;
	Power3 power;
};

class Scene{
public:
	std::vector<Triangle> triangleArray;
	std::vector<Light> lightArray;
	std::vector<Sphere> sphereArray;
	Scene(){}
};

class Camera{
public:
	float zNear;
	float zFar;
	float fieldOfViewX;

	Camera() : zNear(-1.0f), zFar(-100.0f), fieldOfViewX(M_PI/2.0f){}
};

