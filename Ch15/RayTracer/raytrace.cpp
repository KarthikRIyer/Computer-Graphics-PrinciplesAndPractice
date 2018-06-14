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

class BSDF{
public:
	Color3 k_L;
	Color3 k_G;
	float sharpness = 100.0f;
	BSDF(){}
	BSDF(Color3 lambert, Color3 gloss, float sharp):k_L(lambert),k_G(gloss),sharpness(sharp){}

	Color3 evaluateFiniteScatteringDensity(Vector3& w_i, Vector3& w_o, Vector3& n) const{
		Vector3 w_h = (w_i + w_o).direction();
		return (k_L + k_G*((sharpness+8.0f)*powf(std::max(0.0f,w_h.dot(n)),sharpness)/8.0f))/M_PI;
	}
};

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

class Sphere{
public:
	Point3 Q;
	float rad;
	BSDF m_bsdf;
	Sphere(){}
	Sphere(Point3 center,float r,BSDF bsdf):Q(center),rad(r),m_bsdf(bsdf){}
	Point3 center()const{return Q;}
	float radius()const{return rad;}
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

Ray computeEyeRay(float x, float y, int width, int height, const Camera& camera){

	const float aspect = float(height)/float(width);

	const float s = -2.0*tan(camera.fieldOfViewX*0.5f);

	Vector3 start = Vector3(-(x/width - 0.5f)*s , (y/height - 0.5f)*s*aspect,1.0f*camera.zNear);

	return Ray(start,start.direction());

}

float intersectT(Ray& R,const Triangle& T, float weight[3]){

	Vector3 e1 = T.vertex(1) - T.vertex(0);
	Vector3 e2 = T.vertex(2) - T.vertex(0);
	Vector3 q = R.direction().cross(e2);

	float  a = e1.dot(q);

	Vector3 s = R.origin() - T.vertex(0);
	Vector3 r = s.cross(e1);

	weight[1] = s.dot(q) / a;
	weight[2] = R.direction().dot(r) / a;
	weight[0] = 1 - (weight[1]+weight[2]);

	const float dist = e2.dot(r) / a;

	static const float epsilon = 1e-7f;
	static const float epsilon2 = 1e-10f;

	if((a<=epsilon) || (weight[0]<-epsilon2) || (weight[1]<-epsilon2) || (weight[2]<-epsilon2) || (dist<=0.0f)){
		return INFINITY;
	}else{
		return dist;
	}
}

float intersectS(Ray& R, const Sphere& S){
	Vector3 v = R.origin() - S.center();
	if(v.length() > S.radius()){
	float a = (R.direction()).dot(R.direction());
	float b = 2.0f*((R.direction()).dot(v));
	float c = v.dot(v) - S.radius();
	float D = b*b - 4*a*c;
	if(D>=0){
		float t1 = (-b + sqrt(D))/(2*a);
		float t2 = (-b - sqrt(D))/(2*a);
		if(t1<0 && t2>0){t1 = t2;}
		if(t1>0 && t2<0){t2 = t1;}
		if(t1<0 && t2<0){return INFINITY;}
		float d1 = (R.direction()*t1).length();
		float d2 = (R.direction()*t2).length();

		return std::min(d1,d2);
	}
	}
	return INFINITY;
}

bool visible(Point3& P, Vector3& direction, float distanceToLight, const Scene& scene){

	static const float rayBumpEpsilon = 1e-4;
	Ray shadowRay = Ray(P+(direction*rayBumpEpsilon),direction);
	distanceToLight -= rayBumpEpsilon;
	float ignore[3];
	for(unsigned int i = 0;i < scene.triangleArray.size();++i){
		if(intersectT(shadowRay,scene.triangleArray[i],ignore) < distanceToLight){
			return false;
		}
	}
	for(unsigned int i = 0; i < scene.sphereArray.size();++i){
		if(intersectS(shadowRay, scene.sphereArray[i]) <  distanceToLight){
			return false;
		}
	}
	return true;
}


void shade(const Scene& scene, Triangle& T,Point3& P, Vector3& n,  Vector3& w_o, Radiance3& L_o){
	L_o = Color3(0.0f,0.0f,0.0f);

	for(unsigned int i =0; i<scene.lightArray.size();i++){
		Light light = scene.lightArray[i];

		Vector3 offset = light.position - P;
		float distanceToLight = offset.length();
		Vector3 w_i = offset/distanceToLight;

		if(visible(P ,w_i ,distanceToLight ,scene)){
			Radiance3 L_i = light.power / (4*M_PI*distanceToLight*distanceToLight);
			
			L_o.r += (L_i*T.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).r*std::max(0.0f,w_i.dot(n));
			L_o.g += (L_i*T.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).g*std::max(0.0f,w_i.dot(n));
			L_o.b += (L_i*T.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).b*std::max(0.0f,w_i.dot(n));
	
		}
	}

}

void shade(const Scene& scene, Sphere& S, Point3& P, Vector3& n, Vector3& w_o, Radiance3& L_o){
	L_o = Color3(0.0f,0.0f,0.0f);

	for(unsigned int i = 0; i<scene.lightArray.size();i++){
		Light light = scene.lightArray[i];

		Vector3 offset = light.position - P;
		float distanceToLight = offset.length();
		Vector3 w_i = offset/distanceToLight;
		if(visible(P ,w_i ,distanceToLight ,scene)){
			Radiance3 L_i = light.power / (4*M_PI*distanceToLight*distanceToLight);

			L_o.r += (L_i*S.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).r*std::max(0.0f,w_i.dot(n));
			L_o.g += (L_i*S.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).g*std::max(0.0f,w_i.dot(n));
			L_o.b += (L_i*S.bsdf().evaluateFiniteScatteringDensity(w_i,w_o,n)).b*std::max(0.0f,w_i.dot(n));
		}
	}
}

bool sampleRayTriangle(const Scene& scene, int x, int y, Ray& R, Triangle& T, Radiance3& radiance, float& distance){
	float weight[3];
	const float d  = intersectT(R,T,weight);

	if(d >= distance){
		return false;
	}

	distance = d;

	Point3 P = R.origin() + R.direction()*d;

	Vector3 n = (T.normal(0)*weight[0] + T.normal(1)*weight[1] + T.normal(2)*weight[2]).direction();

	Vector3 w_o = -R.direction();

	shade(scene, T, P, n, w_o, radiance);

	return true;
}

bool sampleRaySphere(const Scene& scene, int x, int y, Ray& R, Sphere& S, Radiance3& radiance, float& distance){
	const float d = intersectS(R,S);
	if(d >= distance){
		return false;
	}

	distance = d;

	Point3 P = R.origin() + R.direction()*d;
	Vector3 n = (P - S.center()).direction();
	Vector3 w_o = -R.direction();

	shade(scene, S, P, n, w_o, radiance);
	return true;
}

void rayTrace(Image image , const Scene& scene , const Camera& camera , int x0 , int x1 , int y0 , int y1){
	for(int y = y0; y < y1 ; ++y){
		for(int x = x0; x < x1 ; ++x){

			Ray R = computeEyeRay(x + 0.5f , y + 0.5f , image.width() , image.height() , camera);

			float distance = INFINITY;

			 Radiance3 L_o;

			for(unsigned int t = 0; t<scene.triangleArray.size(); ++t){
				Triangle T = scene.triangleArray[t];

				if(sampleRayTriangle(scene, x, y, R, T, L_o, distance)){
					image.set(x,y,L_o);
				}

			}

			for(unsigned int t = 0;t<scene.sphereArray.size();++t){
				Sphere S = scene.sphereArray[t];
				if(sampleRaySphere(scene,x,y,R,S,L_o,distance)){
					image.set(x,y,L_o);
				}
			}			

		}
	}
	image.save("result_4.ppm",2.0f);
}

void lightScene(Scene& scene){

	
	scene.lightArray.resize(2);
	scene.lightArray[0].position = Point3(1, 3, 1);
	scene.lightArray[0].power = Color3(1.0f,1.0f,1.0f)*20.0f;
	scene.lightArray[1].position = Point3(-1, 1, 0);
	scene.lightArray[1].power = Color3(1.0f,1.0f,1.0f)*10.0f;

}

void triangleGroundScene(Scene& scene){
	lightScene(scene);

	scene.triangleArray.push_back(Triangle(Vector3(-0.5,1.7,-3), Vector3(-2.4,-0.5,-3), Vector3(1.1,0,-3),Vector3(0,0.6f,1).direction(),Vector3(-0.4f,-0.4f, 1.0f).direction(),Vector3(0.4f,-0.4f, 1.0f).direction(), BSDF(Color3::green()*0.8,Color3::white()*0.2f,100.0f)));
	scene.triangleArray.push_back(Triangle(Vector3(-2.4,-0.5,-3),Vector3(-0.5,1.7,-3), Vector3(1.1,0,-3),Vector3(0,0.6f,1).direction(),Vector3(-0.4f,-0.4f, 1.0f).direction(),Vector3(0.4f,-0.4f, 1.0f).direction(), BSDF(Color3::green()*0.8,Color3::white()*0.2f,100.0f)));

	scene.triangleArray.push_back(Triangle(Vector3(2.0f,0.1f,-4.0f), Vector3(1.0f,1.0f,-4.0f), Vector3(0.2f,0.1f,-4.0f),Vector3(0,0,1.0f).direction(),Vector3(0,0, 1.0f).direction(),Vector3(0,0, 1.0f).direction(), BSDF(Color3::red()*0.8,Color3::white()*0.2f,100.0f)));
	scene.triangleArray.push_back(Triangle(Vector3(1.0f,1.0f,-4.0f),Vector3(2.0f,0.1f,-4.0f), Vector3(0.2f,0.1f,-4.0f),Vector3(0,0,1.0f).direction(),Vector3(0,0, 1.0f).direction(),Vector3(0,0, 1.0f).direction(), BSDF(Color3::red()*0.8,Color3::white()*0.2f,100.0f)));

	const float groundY = -1.0f;

	scene.triangleArray.push_back(Triangle(Vector3(-10, groundY, -10), Vector3(-10, groundY, -0.01f),Vector3(10, groundY, -0.01f),Vector3::unitY(), Vector3::unitY(), Vector3::unitY(), BSDF(Color3::white()*0.8f,Color3::white()*0.2f,100.0f)));
	scene.triangleArray.push_back(Triangle(Vector3(-10, groundY, -10), Vector3(10, groundY, -0.01f),Vector3(10, groundY, -10),Vector3::unitY(), Vector3::unitY(), Vector3::unitY(), BSDF(Color3::white()*0.8f,Color3::white()*0.2f,100.0f)));

	scene.sphereArray.push_back(Sphere(Point3(0.0f,0.3f,-1.7f),0.1f,BSDF(Color3::blue()*0.8,Color3::white()*0.2f,100.0f)));
	scene.sphereArray.push_back(Sphere(Point3(0.1f,0.5f,-1.2f),0.01f,BSDF(Color3::yellow()*0.8,Color3::white()*0.2f,100.0f)));
}

int main(){
	Image image = Image(800,500);
	Camera camera;
	Scene scene = Scene();		
	triangleGroundScene(scene);
	rayTrace(image,scene,camera,0,800,0,500);
}