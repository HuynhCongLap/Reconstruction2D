#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include "QDebug"
#include "iostream"
#include <QQueue>
#include <QStack>
#include <QtMath>
using namespace std;
// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Vertex
{
    // Class to be enriched, otherwise it would have been simply named as 3DPoint!!
    int _vtriangle_index; // keep a index face of this vertex
    double _x;
    double _y;
    double _z;


public:
    Vertex():_x(),_y(),_z(), _vtriangle_index(-1) {}

    Vertex(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_), _vtriangle_index(-1) {}
    Vertex(float x_, float y_, float z_, int index):_x(x_),_y(y_),_z(z_), _vtriangle_index(index) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    void setFace(int faceIndex){ _vtriangle_index = faceIndex; } // set one of the faces belongs to this vertex
    int getFace() {return _vtriangle_index; } // return the face index that this vertex belongs to
};
class BorderEdge{

    int vertex_one;
    int vertex_two;
    int m_face;
public:
    BorderEdge(){}
    BorderEdge(const BorderEdge &copy){
        vertex_one = copy.one();
        vertex_two = copy.two();
        m_face = copy.face();
    }
    BorderEdge(int one, int two, int face):vertex_one(one),vertex_two(two), m_face(face){}
    bool duplicate(BorderEdge a){
        return (vertex_one == a.one() && vertex_two == a.two() ) || (vertex_one == a.two() && vertex_two == a.one());
    }
public:
    int one() const {return vertex_one;}
    int two() const {return vertex_two;}
    int face()const {return m_face;}
};

//** TO MODIFY
class Triangle
{

    // add  later
    QVector <int> _vertexIndex;
    QVector <int> _triangleIndex;
public:
    Triangle(){}

    Triangle(int a, int b, int c){
        _vertexIndex.push_back(a);
        _vertexIndex.push_back(b);
        _vertexIndex.push_back(c);
    }

    Triangle(int a, int b, int c, int voisin_one, int voisin_two, int voisin_three){
        _vertexIndex.push_back(a);
        _vertexIndex.push_back(b);
        _vertexIndex.push_back(c);

        _triangleIndex.push_back(voisin_one);
        _triangleIndex.push_back(voisin_two);
        _triangleIndex.push_back(voisin_three);
    }

    // get
        int a() const { return  _vertexIndex[0]; }
        int b() const { return  _vertexIndex[1]; }
        int c() const { return  _vertexIndex[2]; }
        bool isInfinityFace(){ return _vertexIndex.indexOf(0)!=-1; }

        int voisin1() const { return  _triangleIndex[0]; }
        int voisin2() const { return  _triangleIndex[1]; }
        int voisin3() const { return  _triangleIndex[2]; }

        int getVoisin(int n) {
            if (n > 2 || n < 0)
                return -1;
            else
                return _triangleIndex[n];
          }
        int getVertex(int n){
            if (n > 2 || n < 0)
                return -1;
            else
                return _vertexIndex[n];
        }

        bool testVertex(int i) // check if this face has this vertex inside
            {
                return _vertexIndex.indexOf(i) != -1;
            }

        void attachedVoisin(int a, int b, int c) // set three voisin by index attached triangles
            {
                _triangleIndex[0] = a;
                _triangleIndex[1] = b;
                _triangleIndex[2] = c;
            }


         void changeVoisin(int i, int n)
         {
             int index = _triangleIndex.indexOf(i);
             if(index == -1)
                 return;
             else
                 _triangleIndex[index] = n;
         }

         void changeVoisin(int i, int n, int zero)
         {
             if(i>2 || i<0){
                std::cout<<"Khong doi"<<endl;
                return;
             }
             else
                 _triangleIndex[i] = n;
         }

         int getLast(int x, int y)
         {
             int index1 = _vertexIndex.indexOf(x);
             int index2 = _vertexIndex.indexOf(y);
             if(index1 == -1 || index2 == -1 || index1 == index2)
             {
                 std::cout<<"Khong co canh nay";
                 return -1;
             }
              return 3 -index1 -index2;
         }

         void changeVoisin(BorderEdge be)
         {
             int v_i = vertex_index(be.two());
             changeVoisin(getVoisin(next_vertex_index(v_i)),be.face());
         }

         void changeVertex(int i, int n)
         {
             int index = _vertexIndex.indexOf(i);
             if(index == -1)
                 return;
             else
                 _vertexIndex[index] = n;
         }

         int vertex_index(int n)
         {
             return _vertexIndex.indexOf(n);
         }

         int voisin_index(int n)
         {
             return _triangleIndex.indexOf(n);
         }
         int next_vertex_index(int i)
         {
             if (i > 2 || i < 0)
                 return -1;
             else
             {
                 i++;
                 if(i>2)
                     return 0;
                 else
                     return i;
             }
         }
};


//** TO MODIFY
class Mesh
{
    QVector<Vertex> _vertexArray;

    QVector<Vertex> _baryCenter;
    QVector<Triangle> _facesArray;

    QVector<int> _infinityFaces;
    QStack<BorderEdge> _borderEgde;

    QVector<Vertex> _terrain;
    int current = 0;
public:
    Mesh();
    // Bouding box constructor

    void universe();

    void insertTerrain()
    {
        newInsertMesh(_terrain[current]);
        current++;
    }
    void oneTriangleInit()
    {
        _vertexArray.resize(0);
        _facesArray.resize(0);
        _infinityFaces.resize(0);
        _borderEgde.resize(0);

        Vertex infinite(5,5,-5,1);
        Vertex one(0,0,0,0);
        Vertex two(0,-5,0,0);
        Vertex three(5,-5,0,0);
        Vertex four(5,0,0,0);

        _facesArray.resize(0);
        _facesArray.push_back(Triangle(1,2,3,4,1,3));
        _facesArray.push_back(Triangle(1,3,4,5,2,0));
        _facesArray.push_back(Triangle(0,4,1,1,3,5));
        _facesArray.push_back(Triangle(0,1,2,0,4,2));
        _facesArray.push_back(Triangle(0,2,3,0,5,3));
        _facesArray.push_back(Triangle(0,3,4,1,2,4));


        _vertexArray.push_back(infinite);
        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);
        _vertexArray.push_back(four);

        _infinityFaces.push_back(2);
        _infinityFaces.push_back(3);
        _infinityFaces.push_back(4);
        _infinityFaces.push_back(5);
    }
    // Check a point inside a triangle
    void boudingBox(){
        Vertex one(-5,5,0,0);
        Vertex two(-5,-5,0,0);
        Vertex three(5,-5,0,1);
        Vertex four(5,5,0,1);

        Triangle face_one(0,1,2,-1,1,-1);
        Triangle face_two(2,3,0,-1,0,-1);

        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);
        _vertexArray.push_back(four);

        _facesArray.push_back(face_one);
        _facesArray.push_back(face_two);

        _borderEgde.push_back(BorderEdge(0,1,0));
        _borderEgde.push_back(BorderEdge(1,2,0));
        _borderEgde.push_back(BorderEdge(2,3,1));
        _borderEgde.push_back(BorderEdge(3,0,1));

    }
    float sign (Vertex p1, Vertex p2, Vertex p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    float signxz (Vertex p1, Vertex p2, Vertex p3)
    {
        return (p1.x() - p3.x()) * (p2.z() - p3.z()) - (p2.x() - p3.x()) * (p1.z() - p3.z());
    }

    bool PointInTriangle (Vertex pt, Vertex v1, Vertex v2, Vertex v3)
    {
        bool b1, b2, b3;

        b1 = sign(pt, v1, v2) < 0.0f;
        b2 = sign(pt, v2, v3) < 0.0f;
        b3 = sign(pt, v3, v1) < 0.0f;

        return ((b1 == b2) && (b2 == b3));
    }


    bool testVoisin(Triangle one, Triangle two) // test if 2 triangle is voisin
    {
        int a1 = one.a();
        int b1 = one.b();
        int c1 = one.c();

        int a2 = two.a();
        int b2 = two.b();
        int c2 = two.c();
        bool flag1 = (a1 == a2 || a1 == b2 || a1 == c2) && (b1 == a2 || b1 == b2 || b1 == c2);
        bool flag2 = (a1 == a2 || a1 == b2 || a1 == c2) && (c1 == a2 || c1 == b2 || c1 == c2);
        bool flag3 = (c1 == a2 || c1 == b2 || c1 == c2) && (b1 == a2 || b1 == b2 || b1 == c2);

        return flag1 || flag2 || flag3;
    }

    // split a triangle index n into 3 with a point



    //-----------------------------------------------------
    bool newSplitTriangle(Vertex pt, int index_face);
    int newInsertMesh(Vertex pt)
    {
        for(int i=0 ; i<_facesArray.size(); i++)
        {

            Vertex point1 = _vertexArray[_facesArray[i].a()];
            Vertex point2 = _vertexArray[_facesArray[i].b()];
            Vertex point3 = _vertexArray[_facesArray[i].c()];
            if(PointInTriangle(pt,point1,point2,point3))
                {
                    newSplitTriangle(pt,i);
                     return 1; // insert that inside outside
                }

        }
            insertPointOutSide(pt);
            return 0 ; // insert that point outside

     }


    void readPointFromFile(QString fileName);


    void flip(int triangle_index, int vertex)
    {
       // Triangle face_voisin = _facesArray[triangle_index]
          int vertex_index = _facesArray[triangle_index].vertex_index(vertex);
          if(vertex_index == -1){
            cout << "This face has no this vertex"<<endl;
            return;
          }
          else
          {

              int voisin = _facesArray[triangle_index].getVoisin(vertex_index);
              if(voisin == -1)
                  return;

              int _vf = _facesArray[triangle_index].next_vertex_index(vertex_index);
              int _vs = _facesArray[triangle_index].next_vertex_index(_vf);

              int point_two = _facesArray[triangle_index].getVertex(_vf);
              int point_three = _facesArray[triangle_index].getVertex(_vs);

              int voisin_vertex_index = _facesArray[voisin].voisin_index(triangle_index);
              int point_four = _facesArray[voisin].getVertex(voisin_vertex_index);


              int voisin2 = _facesArray[triangle_index].getVoisin(_vf);

              int _vvs = _facesArray[voisin].next_vertex_index(voisin_vertex_index);
              int voisin3 = _facesArray[voisin].getVoisin(_vvs);

              if(voisin2 != -1)
              _facesArray[voisin2].changeVoisin(triangle_index,voisin);
               if(voisin3 !=-1)
              _facesArray[voisin3].changeVoisin(voisin,triangle_index);


              _facesArray[triangle_index].changeVertex(point_three,point_four);
              _facesArray[triangle_index].changeVoisin(voisin2,voisin3);

              _facesArray[voisin].changeVertex(point_two,vertex);
              _facesArray[voisin].changeVoisin(voisin3,voisin2);

          }

    }
    // The two following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh

    float predicate_delau(int ti, int v_index)
    {
        int voisin  = _facesArray[ti].getVoisin(v_index);
        cout<<"voisin: "<<voisin<<endl;
        cout<<_facesArray[1].getVoisin(0)<<endl;
        cout<<_facesArray[1].getVoisin(1)<<endl;
        cout<<_facesArray[1].getVoisin(2)<<endl;
        if(voisin == -1)
            return -3.14;

        int v2 = _facesArray[ti].next_vertex_index(v_index);
        int v3 = _facesArray[ti].next_vertex_index(v2);

        Vertex Q = _vertexArray[_facesArray[ti].getVertex(v_index)];
        Vertex R = _vertexArray[_facesArray[ti].getVertex(v2)];
        Vertex P = _vertexArray[_facesArray[ti].getVertex(v3)];
        cout<<"her: "<<v_index<<endl;
        int v4 = _facesArray[voisin].voisin_index(ti);
         cout<<"her1:"<<v4<<endl;
        Vertex S = _vertexArray[_facesArray[_facesArray[ti].getVoisin(v_index)].getVertex(v4)];
        cout<<"her2 "<<endl;
        /*Compute vector PQ, PR, PS*/
        float PQx = Q.x()-P.x();
        float PQy = Q.y()-P.y();
        float PRx = R.x()-P.x();
        float PRy = R.y()-P.y();
        float PSx = S.x()-P.x();
        float PSy = S.y()-P.y();
        float PQ2 = PQx*PQx+PQy*PQy;
        float PR2 = PRx*PRx+PRy*PRy;
        float PS2 = PSx*PSx+PSy*PSy;
        /*Compute Predicate*/
        float predi = PQx*PRy*(PS2)+PRx*PSy*(PQ2)+PSx*PQy*(PR2)-PQx*PSy*(PR2)-PRx*PQy*(PS2)-PSx*PRy*(PQ2);
        return predi;
    }
    void check_predicate()
    {
        for(int i=0; i<_facesArray.size(); i++)
        {
            cout<<"Face "<< i<<endl;
            cout<<predicate_delau(i,0)<<endl;
            cout<<predicate_delau(i,1)<<endl;
            cout<<predicate_delau(i,2)<<endl;
        }
    }
    void insertPointOutSide(Vertex vt);
    void draw();
    void draw_universe();
    void drawTetraedra();
    void drawTetraedraWireFrame();

    float AngleBetweenThreePoints(Vertex c, Vertex b, Vertex a)
    {
            float abx = b.x() - a.x() ;
            float aby = b.y() - a.y() ;
            float cbx = b.x() - c.x() ;
            float cby = b.y() - c.y();

            float dot = (abx * cbx + aby * cby); // dot product
            float cross = (abx * cby - aby * cbx); // cross product

            float alpha = atan2(cross, dot);

            return alpha;
    }

    void Angle(){

        _baryCenter.resize(0);
        for(int i=0; i<_facesArray.size(); i++){
            Vertex A = _vertexArray[_facesArray[i].getVertex(0)];
            Vertex B = _vertexArray[_facesArray[i].getVertex(1)];
            Vertex C = _vertexArray[_facesArray[i].getVertex(2)];
            float tanA = tan(AngleBetweenThreePoints(C,A,B));
            float tanB = tan(AngleBetweenThreePoints(A,B,C));
            float tanC = tan(AngleBetweenThreePoints(B,C,A));

             Vertex H(tanB + tanC, tanC + tanA, 0);
            _baryCenter.push_back(H);
        }
        cout<<_baryCenter.size();
    }
};

#endif // MESH_H
