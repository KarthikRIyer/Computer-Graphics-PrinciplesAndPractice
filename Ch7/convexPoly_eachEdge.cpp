#include<iostream>
#include<vector>
#include<math.h>

using namespace std;

class Vector2 { 
	public: float x,y; 
			float dot(Vector2 v){
				return (x * v.x + y * v.y);
			}
			Vector2 operator - (Vector2 v) const
    		{ return Vector2(x - v.x, y - v.y); }
    		Vector2 operator + (Vector2 v) const
    		{ return Vector2(x + v.x, y + v.y); }
    		Vector2 operator / (float f) const
    		{ return Vector2(x/f,y/f); }

    		Vector2(){}
    		Vector2(float a, float b):x(a),y(b){}
    		Vector2 direction(){
				return Vector2(x/sqrt(x*x+y*y),y/sqrt(x*x+y*y));
					}
};
typedef Vector2 Point2;

int main(){

	Vector2 givenP = Vector2(0.5f,2.0f);

	vector<Point2> p;
	p.push_back(Point2(1,1));
	p.push_back(Point2(1,-1));
	p.push_back(Point2(-1,-1));
	p.push_back(Point2(-1,1));

	int c = 0;
	for(int i = 0; i< p.size(); i++){

		if(i+1 == p.size()){

			Vector2 v1 = p[0] - p[i];
			float a = (-(v1.x)/v1.y);
			Vector2 n = Vector2(-(v1.y),v1.x);
			Vector2 normal = n.direction();
			Vector2 dir = ((p[0] + p[i])/2.0f) - givenP;
			if(dir.dot(normal) > 0){c++;}

		}else{

			Vector2 v1 = p[i+1] - p[i];
			float a = (-(v1.x)/v1.y);
			Vector2 n = Vector2(-(v1.y),v1.x);
			Vector2 normal = n.direction();
			Vector2 dir = ((p[i+1] + p[i])/2) - givenP;
			if(dir.dot(normal) > 0){c++;}
			}

	}
	if(c == p.size()){cout<<"Point inside polygon"<<endl;}
	else{cout<<"Point outside polygon"<<endl;}

}