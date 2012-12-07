// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

// the number type
#include <CGAL/MP_Float.h>

typedef CGAL::MP_Float NT;

// choose the kernel
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<NT>  Kernel;

// typedefs for the traits and the algorithm

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Apollonius_graph_traits_2.h>

typedef CGAL::Apollonius_graph_traits_2<Kernel>   Traits;
//typedef CGAL::Apollonius_graph_2<Traits>          Apollonius_graph;

// choose the representation
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Rep;

#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// Qt stuff

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>

typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>   Vb;
typedef CGAL::Triangulation_face_base_2<Traits>              Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>       Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>    Apollonius_graph;

typedef Agds::Face_iterator All_faces_iterator;
typedef Agds::Face Face;
typedef Agds::Vertex_handle      Vertex_handle;
typedef Agds::Vertex      Vertex;

int main(int argc , char* argv[])
{
  std::ifstream ifs("../data/sites.cin");
  assert( ifs );

  Apollonius_graph ag;
  Apollonius_graph::Site_2 site;

  int k = 0;
  // read the sites and insert them in the Apollonius graph
  while ( ifs >> site ) {
    std::cout << "inserting: " << ++k << std::endl;
    std::cout << site << std::endl;
    ag.insert(site);
    size_t nof = ag.number_of_faces ();
    std::cout << "number of faces: " << nof << std::endl;
  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  size_t nof = ag.number_of_faces ();
  std::cout << "number of faces: " << nof << std::endl;

  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    Face face = *fiter;
    Vertex_handle vertex = face.vertex(0);
    Vertex v = *vertex;
    //v.point();
    //std::cout << "face: " << face << std::endl;
    //std::cout << "vertex: " << vertex.point() << std::endl;
    std::cout << "vertex: " << v << std::endl;
  }

  /*
   * visualize with Qt
   */

  QApplication app(argc, argv);
  // Prepare scene
  QGraphicsScene scene;
  scene.setSceneRect(-400, -400, 800, 800); //Has the format: (x,y,width,height)

  scene.addLine(
    QLineF(100, 100, 200, 300),
    QPen(Qt::green, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
  );

  // Prepare view, add scene, show
  QGraphicsView* view = new QGraphicsView(&scene);
  //view->show();

  // Add CGAL's navigation filter
  CGAL::Qt::GraphicsViewNavigation navigation;
  view->installEventFilter(&navigation);
  view->viewport()->installEventFilter(&navigation);
  view->setRenderHint(QPainter::Antialiasing);

  // create an image
  QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  scene.render(&painter);
  image.save("image.png");
 
  //return app.exec();  
}
