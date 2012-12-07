// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

//#include <map>

// example that uses the filtered traits

// choose the representation
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Rep;

#include <CGAL/Apollonius_graph_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Apollonius_graph_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Apollonius_graph_filtered_traits_2.h>

#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Apollonius_graph_adaptation_traits_2.h>
#include <CGAL/Apollonius_graph_adaptation_policies_2.h>

// Qt includes

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QGraphicsView>
#include <QLineF>

// typedef for the traits; the filtered traits class is used
typedef CGAL::Apollonius_graph_filtered_traits_2<Rep> Traits;

// typedefs for the algorithm

// With the second template argument in the vertex base class being
// false, we indicate that there is no need to store the hidden sites.
// One case where this is indeed not needed is when we only do
// insertions, like in the main program below.
typedef CGAL::Apollonius_graph_vertex_base_2<Traits,false>	Vb;
typedef CGAL::Triangulation_face_base_2<Traits>         	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>		Agds;
typedef CGAL::Apollonius_graph_2<Traits,Agds>    		Apollonius_graph;

typedef Agds::Face 					Face;
typedef Agds::Face_handle				Face_handle;
typedef Agds::Vertex      				Vertex;
typedef Agds::Vertex_handle     			Vertex_handle;

typedef Agds::Face_iterator 				All_faces_iterator;
typedef Agds::Edge_iterator 				All_edges_iterator;
typedef Agds::Vertex_iterator 				All_vertices_iterator;
typedef Apollonius_graph::Finite_faces_iterator		Finite_faces_iterator;
typedef Apollonius_graph::Finite_edges_iterator		Finite_edges_iterator;
typedef Apollonius_graph::Finite_vertices_iterator	Finite_vertices_iterator;
typedef Agds::Face_circulator 				Face_circulator;

typedef Traits::Object_2				Object_2;
typedef Traits::Site_2					Site_2;
typedef Traits::Point_2					Point_2;
typedef Traits::Line_2					Line_2;

typedef CGAL::Apollonius_graph_adaptation_traits_2<Apollonius_graph>			AT;
typedef CGAL::Apollonius_graph_degeneracy_removal_policy_2<Apollonius_graph>		AP;
typedef CGAL::Voronoi_diagram_2<Apollonius_graph,AT,AP>				Voronoi_diagram;

int main(int argc , char* argv[])
{
  std::ifstream ifs("../data/sites.cin");
  assert( ifs );

  Apollonius_graph ag;
  Apollonius_graph::Site_2 site;

  int k = 0;
  // read the sites and insert them in the Apollonius graph
  while ( ifs >> site ) {
    std::cout << "inserting: " << ++k << ": " << site << std::endl;
    std::cout << "position: " << site.point() << ", weight: " << site.weight() << std::endl;
    ag.insert(site);
    size_t nof = ag.number_of_faces ();
    std::cout << "number of faces: " << nof << std::endl;
  }

  // validate the Apollonius graph
  assert( ag.is_valid(true, 1) );
  std::cout << std::endl;

  size_t nof = ag.number_of_faces ();
  std::cout << "number of faces: " << nof << std::endl;

  // create the voronoi diagram
  Voronoi_diagram vd(ag);
  // TODO: yeah, this seems to work. Get the cell from the VD

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

  // add a circle for each site
  for (Finite_vertices_iterator viter = ag.finite_vertices_begin();
    viter != ag.finite_vertices_end(); ++viter) {
    Site_2 site = viter->site();
    Point_2 p = site.point();
    double weight = site.weight();
    double size = weight * 2;

    scene.addEllipse(
      p.x() - weight, p.y() - weight, size, size,
      QPen(Qt::red, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
    );
  }

  // add dual image stubs
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    Object_2 o = ag.dual(fiter);
    Site_2 site;
    Line_2 line;
    if (assign(site, o)) {
      Point_2 p = site.point();
      double size = 6;
      scene.addEllipse(
         p.x() - size/2, p.y() - size/2, size, size,
         QPen(Qt::blue, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
       );
    } else if (assign(line, o)) {
    }
  }

  // add dual image stubs
  for (All_faces_iterator fiter = ag.all_faces_begin (); fiter != ag.all_faces_end(); ++fiter) { 
    Object_2 o = ag.dual(fiter);
    Site_2 site;
    Line_2 line;
    if (assign(site, o)) {
      Point_2 p = site.point();
      double size = 6;
      scene.addEllipse(
         p.x() - size/2, p.y() - size/2, size, size,
         QPen(Qt::blue, 3,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
       );
    } else if (assign(line, o)) {
    }
  }
  for (All_vertices_iterator viter = ag.all_vertices_begin (); viter != ag.all_vertices_end(); ++viter) { 
    //Face_circulator fcirc = ag.incident_faces(viter);
    Face_circulator fcirc = ag.incident_faces(viter), done(fcirc);
    do {
      Object_2 o = ag.dual(fcirc);
    } while(++fcirc != done);
  }

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
  image.save("test5.png");
 
  //return app.exec();  
}
