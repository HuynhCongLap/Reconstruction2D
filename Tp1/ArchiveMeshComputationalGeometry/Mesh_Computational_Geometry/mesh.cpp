#include "mesh.h"
#include <iostream>
#include <QFile>
Mesh::Mesh()
{

}

// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(Vertex vertex) {
    glVertex3f(vertex.x(), vertex.y(), vertex.z());
}

//Example with a tetraedra
void Mesh::drawTetraedra() {
    QVector<Vertex> vertexTab;
    vertexTab.push_back(Vertex(-0.5,-0.5,-0.5)); //0
    vertexTab.push_back(Vertex(0.5,-0.5,-0.5)); // 1
    vertexTab.push_back(Vertex(0,0.5,-0.5)); // 2
    vertexTab.push_back(Vertex(0,-0.5,0.5)); // 3

    int faces[12]={
        0,1,2, // 1
        1,3,2, // 2
        3,0,2, // 3
        0,1,3  // 4
    };

    for(int i = 0; i < 12; i+=3) {

        if (i < 3) glColor3d(1,0,0);
        else if (i < 6) glColor3d(0,1,0);
        else if (i < 9) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        glBegin(GL_TRIANGLES);
        glVertexDraw(vertexTab[faces[i]]);
        glVertexDraw(vertexTab[faces[i+1]]);
        glVertexDraw(vertexTab[faces[i+2]]);
        glEnd();
    }


}


double randd() {
  return (double)rand() / (RAND_MAX + 1.0);
}

void Mesh::draw()
{

    glPointSize(3);
    glLineWidth(3);
    for(int i = 0; i < _facesArray.size(); i++) {

        glColor3d(1,0,0);

        glBegin(GL_POINTS);
        glVertexDraw(_vertexArray[_facesArray[i].a()]);
        glVertexDraw(_vertexArray[_facesArray[i].b()]);
        glVertexDraw(_vertexArray[_facesArray[i].c()]);
        glEnd();
        glColor3d(0,0,1);
        glBegin(GL_LINE_STRIP);
        glVertexDraw(_vertexArray[_facesArray[i].a()]);
        glVertexDraw(_vertexArray[_facesArray[i].b()]);
        glEnd();


       glBegin(GL_LINE_STRIP);
       glVertexDraw(_vertexArray[_facesArray[i].b()]);
       glVertexDraw(_vertexArray[_facesArray[i].c()]);
       glEnd();


       glBegin(GL_LINE_STRIP);
       glVertexDraw(_vertexArray[_facesArray[i].c()]);
       glVertexDraw(_vertexArray[_facesArray[i].a()]);
       glEnd();


       //glColor3d(0,1,0);
      // glBegin(GL_TRIANGLES);
      // glVertexDraw(_vertexArray[_facesArray[i].a()]);
     //  glVertexDraw(_vertexArray[_facesArray[i].b()]);
     //  glVertexDraw(_vertexArray[_facesArray[i].c()]);
      // glEnd();

    }
     glPointSize(6);
     glColor3d(1,0.5,0);
    for(int i=0; i<_baryCenter.size() ; i++)
    {
        glBegin(GL_POINTS);
        glVertexDraw(_baryCenter[i]);
        glEnd();
    }


}


void Mesh::draw_universe()
{

    for(int i = 0; i < _facesArray.size(); i++) {

       glColor3d(0,0.1,0);
       glBegin(GL_TRIANGLES);
       glVertexDraw(_vertexArray[_facesArray[i].a()]);
       glVertexDraw(_vertexArray[_facesArray[i].b()]);
       glVertexDraw(_vertexArray[_facesArray[i].c()]);
       glEnd();

    }


}

//Example with a wireframe tedraedra
void Mesh::drawTetraedraWireFrame() {
    QVector<Vertex> vertexTab;
    vertexTab.push_back(Vertex(-0.5,-0.5,-0.5)); //0
    vertexTab.push_back(Vertex(0.5,-0.5,-0.5)); // 1
    vertexTab.push_back(Vertex(0,0.5,-0.5)); // 2
    vertexTab.push_back(Vertex(0,-0.5,0.5)); // 3

    int faces[12]={
        0,1,2, // 1
        1,3,2, // 2
        3,0,2, // 3
        0,1,3  // 4
    };

    for(int i = 0; i < 12; i+=3) {
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i]]);
            glVertexDraw(vertexTab[faces[i+1]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i+1]]);
            glVertexDraw(vertexTab[faces[i+2]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i+2]]);
            glVertexDraw(vertexTab[faces[i]]);
        glEnd();
    }

}
void Mesh::readPointFromFile(QString fileName)
{
    QFile inputFile(fileName);
        inputFile.open(QIODevice::ReadOnly);
        if (!inputFile.isOpen()){
            std::cout << "Can't not find documents" << std::endl;
            return;
         }

        QTextStream stream(&inputFile);
        QString line = stream.readLine();

        while (!line.isNull()) {

               line = stream.readLine();
               if(line.split(" ", QString::SkipEmptyParts).count() == 3)
               {
                   // read vertex
                   float a = line.split(" ", QString::SkipEmptyParts)[0].toFloat();
                   float b = line.split(" ", QString::SkipEmptyParts)[1].toFloat();
                   float c = line.split(" ", QString::SkipEmptyParts)[2].toFloat();
                   _terrain.push_back(Vertex(a,b,c));

               }
        }
}

bool Mesh::newSplitTriangle(Vertex pt, int index_face)
{

    int pt_index = _vertexArray.size();
    _vertexArray.push_back(pt);

    int vertex1 = _facesArray[index_face].a();
    int vertex2 = _facesArray[index_face].b();
    int vertex3 = _facesArray[index_face].c();

    Triangle one(vertex1,vertex2,pt_index,_facesArray.size(),_facesArray.size()+1,_facesArray[index_face].voisin3());
    Triangle two(pt_index,vertex2,vertex3,_facesArray[index_face].voisin1(),_facesArray.size()+1,index_face);
    Triangle three(vertex1,pt_index,vertex3,_facesArray.size(),_facesArray[index_face].voisin2(),index_face);

    int voisin_1 = _facesArray[index_face].voisin1();
    int voisin_2 = _facesArray[index_face].voisin2();
    int voisin_3 = _facesArray[index_face].voisin3();

    if(voisin_1 != -1)
        _facesArray[voisin_1].changeVoisin(index_face,_facesArray.size());
    if(voisin_2 != -1)
    _facesArray[voisin_2].changeVoisin(index_face,_facesArray.size()+1);


    _facesArray[index_face] = one;
    _facesArray.push_back(two);
    _facesArray.push_back(three);

    return true;

}
void Mesh::insertPointOutSide(Vertex vt)
{
    QVector<BorderEdge> queue;

    _vertexArray.push_back(vt);
    QStack<BorderEdge> save;
    while(!_borderEgde.isEmpty())
    {
        BorderEdge head (_borderEgde.pop());
       // cout<<"Debug ne: "<<head.one()<<" VA " <<head.two()<<endl;
        save.push(head);

        if(sign(vt,_vertexArray[head.one()],_vertexArray[head.two()]) < 0)
           {
                save.pop();
                BorderEdge one(_vertexArray.size()-1,head.two(),_facesArray.size());
                BorderEdge two(head.one(),_vertexArray.size()-1,_facesArray.size());

                Triangle new_one(_vertexArray.size()-1,head.two(),head.one(),head.face(),-1,-1);
                _facesArray.push_back(new_one);
                int index_last = _facesArray[head.face()].getLast(head.one(), head.two());
               // _facesArray[head.face()].changeVoisin(index_last,_facesArray.size()-1,0);

                if(queue.isEmpty())
                {
                    queue.push_back(one);
                    queue.push_back(two);
                }
                else{
                  bool dup = false;
                  for(int j =0; j<queue.size(); j++)
                  {
                      //  cout<<"Canh 5:" << one.one() <<" Canh 2: " << one.two();
                      if(queue[j].duplicate(one))
                      {

                        _facesArray[queue[j].face()].changeVoisin(one);
                        _facesArray[one.face()].changeVoisin(queue[j]);
                        queue.erase(queue.begin()+j);
                        dup = true;
                        break;
                      }
                  }

                  if(!dup)
                      queue.push_back(one);
                  dup =false;
                  for(int j =0; j<queue.size(); j++)
                  {
                      if(queue[j].duplicate(two))
                      {
                          _facesArray[queue[j].face()].changeVoisin(two);
                          _facesArray[two.face()].changeVoisin(queue[j]);
                          queue.erase(queue.begin()+j);
                          dup = true;
                          break;
                      }
                  }
                  if(!dup)
                      queue.push_back(two);
                }

           }
    }

    for(int i=0; i<save.size(); i++)
    {

        cout<<"save["<<i<<"]: "<<save[i].one()<<" VA " <<save[i].two()<<endl;
        cout<<endl;
    }

    _borderEgde = save;
    _borderEgde.push(queue[0]);
    _borderEgde.push(queue[1]);

  //  cout<<"Queue co van de 0: "<<queue[0].one() <<" and " <<queue[0].two()<<endl;
  //  cout<<"Queue co van de: "<<queue[1].one() <<" and " <<queue[1].two();





}
void Mesh::universe(){
    Vertex one(-100,100,0,0);
    Vertex two(-100,-100,0,0);
    Vertex three(100,-100,0,1);
    Vertex four(100,100,0,1);

    Triangle face_one(0,1,2,-1,1,-1);
    Triangle face_two(2,3,0,-1,0,-1);

    _vertexArray.push_back(one);
    _vertexArray.push_back(two);
    _vertexArray.push_back(three);
    _vertexArray.push_back(four);

    _facesArray.push_back(face_one);
    _facesArray.push_back(face_two);
}
