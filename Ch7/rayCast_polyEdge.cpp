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

class Edge{

public:
    Point2 p1;
    Point2 p2;
    Edge(){}
    Edge(Point2 a, Point2 b):p1(a), p2(b){}

};

vector<Point2> pL;
vector<Point2> pR;
vector<Edge> edgeR;
vector<Edge> edgeL;

float intersect(Edge edge, Point2 gP){

    float t = (gP.y - edge.p1.y)/(edge.p2.y - edge.p1.y);
    return t;

}

bool binaryIntersection(vector<Edge> edges,Point2 gP, int l, int r,int d){
    while(l<=r){
        int mid = l+(r-1)/2;
        float t = intersect(edges[mid],gP);
        if(t>=0 && t<=1){
            float x = (1-t)*edges[mid].p1.x + t*edges[mid].p2.x;
            if(d == 1 && x>gP.x){return true;}
            else if(d == -1 && x<gP.x){return true;}
            else{return false;}
        }
        if(t<0){
            l = mid - 1;
        }
        if(t>1){
            l = mid + 1;
        }
    }
    return false;
}

int main(){

    Point2 gP = Point2(.0f,0.0f);

    vector<Point2> p;
    p.push_back(Point2(0.0f,1.0f));
    p.push_back(Point2(sqrt(3)/2.0f,0.5f));
    p.push_back(Point2(sqrt(3)/2.0f,-0.5f));
    p.push_back(Point2(0.0f,-1.0f));
    p.push_back(Point2(-sqrt(3)/2.0f,-0.5f));
    p.push_back(Point2(-sqrt(3)/2.0f,0.5f));
    

    for(int i = 0; i<p.size(); i++){
        for(int j =i+1; j<p.size(); j++){

            if(p[i].y > p[j].y){
                Point2 temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }

        }
    }

    pL.push_back(p[0]);
    pR.push_back(p[0]);
    for(int i  = 1;i<p.size();i++){

        if(p[i].x >= p[0].x){pR.push_back(p[i]);}
        else {pL.push_back(p[i]);}

    }

    pL.push_back(pR[pR.size()-1]);

    if(gP.x >= p[0].x){
        for(int i  = 0 ; i<pR.size()-1;i++){
            edgeR.push_back(Edge(pR[i],pR[i+1]));
        }

        bool in = binaryIntersection(edgeR,gP,0,edgeR.size()-1,1);
        if(in){cout<<"Point inside polygon."<<endl;}
        else{cout<<"Point outside polygon."<<endl;}

    }else{
        for(int i = 0;i<pL.size()-1;i++){
            edgeL.push_back(Edge(pL[i],pL[i+1]));
        }
        bool in = binaryIntersection(edgeL,gP,0,edgeL.size()-1,-1);
        if(in){cout<<"Point inside polygon."<<endl;}
        else{cout<<"Point outside polygon."<<endl;}
    }



}