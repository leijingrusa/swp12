// vim: si:ts=4:sw=4:expandtab

/*
 * TODO: implement own version of "oriented_side"
 * TODO: implement own version of "has_on"
 *
 */

#include <CGAL/Cartesian.h>
#include <vector>

/*
 * some convenience functions for printing
 */

void echo (const char * text){
     std::cout << text << std::endl;
}

void newline (){
     std::cout << std::endl;
}

/*
 * CGAL boilerplate code
 */

// Kernel type
typedef CGAL::Cartesian<double> Kernel;

// Point type
typedef Kernel::Point_2 Point_2;

// Segment type
typedef Kernel::Segment_2 Segment_2;

// Line type
typedef Kernel::Line_2 Line_2;

// Orientation enum
typedef CGAL::Oriented_side Oriented_side;

/*
 * geometric functions
 * these use predicates and constructions
 */

// test whether two segments intersect
bool intersection1a(Segment_2 s1, Segment_2 s2){
    //check for intersection of to segments
    return do_intersect(s1, s2);
}

// compute the intersection of two segments
CGAL::Object intersection2a(Segment_2 s1, Segment_2 s2){
    //compte the actual intersection
    Kernel::Intersect_2 intersection;
    return intersection(s1, s2);
}

// test whether a point lies on a segment
bool on(Segment_2 s, Point_2 p) {
    //check whether the point lies on the segment
    return do_intersect(s, p);
}

/*
 * helper functions that calculate something and then print the results
 * these use predicates and constructions
 */

void printSegments(Segment_2 s1, Segment_2 s2){
    std::cout << "s1: " << s1 << " s2: " << s2 << std::endl;
}

void testForIntersectionAndPrint(Segment_2 s1, Segment_2 s2){
    bool intersects = intersection1a(s1, s2);
    std::cout << "Do s1 and s2 intersect? Computer says: " << intersects << std::endl;
}

void doIntersectionAndPrint(Segment_2 s1, Segment_2 s2){
    CGAL::Object result = intersection2a(s1, s2);

    if (const CGAL::Point_2<Kernel> *ipoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
        // handle the point intersection case with *ipoint.
        std::cout << "Where do s1 and s2 intersect? Computer says: " << *ipoint << std::endl;
    } else {
        if (const CGAL::Segment_2<Kernel> *iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
            // handle the segment intersection case with *iseg.
            std::cout << "Where do s1 and s2 intersect? Computer says: " << *iseg << std::endl;
        } else {
            // handle the no intersection case.
            std::cout << "Where do s1 and s2 intersect? Computer says: nowhere" << std::endl;
        }
    }
}

/*
 * geometric functions
 * these DO NOT use predicates and constructions
 */

bool intersection1b(Segment_2 s1, Segment_2 s2){
    //check for intersection of to segments
    // get start end end points of segments
    Point_2 p1 = s1.source();
    Point_2 p2 = s1.target();
    Point_2 p3 = s2.source();
    Point_2 p4 = s2.target();
    // create line objects from the points
    Line_2 l1(p1, p2);
    Line_2 l2(p3, p4);
    // compute orientation of each point relative to the other line
    Oriented_side side1 = l1.oriented_side(p3);
    Oriented_side side2 = l1.oriented_side(p4);
    Oriented_side side3 = l2.oriented_side(p1);
    Oriented_side side4 = l2.oriented_side(p2);
//    std::cout << "side " << side1 << std::endl;
//    std::cout << "side " << side2 << std::endl;
//    std::cout << "side " << side3 << std::endl;
//    std::cout << "side " << side4 << std::endl;
    // if any point is on the respective other line
    if (side1 == CGAL::ON_ORIENTED_BOUNDARY
        || side2 == CGAL::ON_ORIENTED_BOUNDARY
        || side3 == CGAL::ON_ORIENTED_BOUNDARY
        || side4 == CGAL::ON_ORIENTED_BOUNDARY) {
        // then we check all possible cases of points on segment
        if (s1.has_on(p3)) {
            return 1;
        }
        if (s1.has_on(p4)) {
            return 1;
        }
        if (s2.has_on(p1)) {
            return 1;
        }
        if (s2.has_on(p2)) {
            return 1;
        }
    }
    // otherwise perform the check for non-colinear segments
    return side1 != side2 && side3 != side4;
}

CGAL::Object intersection2b(Segment_2 s1, Segment_2 s2){
    // if there is no intersection, just return an empty object
    if (!intersection1b(s1, s2)) {
        return CGAL::Object();
    }
    // well, let's find out what kind of intersection this is

    // get start end end points of segments
    Point_2 p1 = s1.source();
    Point_2 p2 = s1.target();
    Point_2 p3 = s2.source();
    Point_2 p4 = s2.target();
    // create line objects from the points
    Line_2 l1(p1, p2);
    Line_2 l2(p3, p4);
    // compute orientation of each point relative to the other line
    Oriented_side side1 = l1.oriented_side(p3);
    Oriented_side side2 = l1.oriented_side(p4);
    Oriented_side side3 = l2.oriented_side(p1);
    Oriented_side side4 = l2.oriented_side(p2);

    // count the number of points that are on the respective other line
    int onCount = 0;
    onCount += side1 == CGAL::ON_ORIENTED_BOUNDARY;
    onCount += side2 == CGAL::ON_ORIENTED_BOUNDARY;
    onCount += side3 == CGAL::ON_ORIENTED_BOUNDARY;
    onCount += side4 == CGAL::ON_ORIENTED_BOUNDARY;

    if (onCount == 0) {
        // simple, non-colinear intersection
        // TODO: implement this
        Point_2 p = Point_2(1,1);    
        return make_object(p);
    }

    // collect points that are on the other segment
    std::vector<Point_2> onPoints;
    if (s1.has_on(p3)) {
        onPoints.push_back(p3);
    }
    if (s1.has_on(p4)) {
        onPoints.push_back(p4);
    }
    if (s2.has_on(p1)) {
        onPoints.push_back(p1);
    }
    if (s2.has_on(p2)) {
        onPoints.push_back(p2);
    }
    std::cout << "size: " << onPoints.size() << std::endl;

    if (onPoints.size() == 1) {
        // one point lies on the other line
        Point_2 p = onPoints.at(0);
        return make_object(p);
    }

    if (onPoints.size() >= 2) {
        // lines are colinear.
        // take the first two entries in the onPoints vector.
        // this should be ok, even if there are more than 2 points in the
        // vector because of the order in which we fill that vector.
        Point_2 p = onPoints.at(0);
        Point_2 q = onPoints.at(1);
        Segment_2 s(p, q);
        return make_object(s);
    }

    // in principal this is impossible
    return CGAL::Object();
}

/*
 * helper functions that DO NOT use predicates or constructions
 */

void testForIntersectionAndPrint2(Segment_2 s1, Segment_2 s2){
    bool intersects = intersection1b(s1, s2);
    std::cout << "Do s1 and s2 intersect? Computer says: " << intersects << std::endl;
}

void doIntersectionAndPrint2(Segment_2 s1, Segment_2 s2){
    CGAL::Object result = intersection2b(s1, s2);

    if (const CGAL::Point_2<Kernel> *ipoint = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result)) {
        // handle the point intersection case with *ipoint.
        std::cout << "Where do s1 and s2 intersect? Computer says: " << *ipoint << std::endl;
    } else {
        if (const CGAL::Segment_2<Kernel> *iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result)) {
            // handle the segment intersection case with *iseg.
            std::cout << "Where do s1 and s2 intersect? Computer says: " << *iseg << std::endl;
        } else {
            // handle the no intersection case.
            std::cout << "Where do s1 and s2 intersect? Computer says: nowhere" << std::endl;
        }
    }
}

/*
 * main function
 */

int main( int argc, char* argv[]) {

    // create some points in the plane
    Point_2 p1(1,2), p2(4,2);
    Point_2 p3(2,1), p4(4,3);
    Point_2 p5(2,2), p6(6,2);
    Point_2 p7(1,4), p8(2,3);
    Point_2 p9(3,2), p10(5,0);

    // create some segments
    Segment_2 s1(p1, p2);
    Segment_2 s2(p3, p4);
    Segment_2 s3(p5, p6);
    Segment_2 s4(p7, p8);
    Segment_2 s5(p9, p10);

    // test with predicate functions
    echo("*****************************************************************");
    echo("Segment 1 and 2, they should intersect at point (3,2)");
    printSegments(s1, s2);
    testForIntersectionAndPrint(s1, s2);
    doIntersectionAndPrint(s1, s2);
    newline();

    echo("Segment 1 and 3, they should intersect at segment (2,2), (4,2)");
    printSegments(s1, s3);
    testForIntersectionAndPrint(s1, s3);
    doIntersectionAndPrint(s1, s3);
    newline();

    echo("Segment 1 and 4, they should not intersect");
    printSegments(s1, s4);
    testForIntersectionAndPrint(s1, s4);
    doIntersectionAndPrint(s1, s4);
    newline();

    echo("Segment 2 and 5, they should intersect at point (3,2)");
    printSegments(s2, s5);
    testForIntersectionAndPrint(s2, s5);
    doIntersectionAndPrint(s2, s5);
    newline();

    // test with non-predicate functions
    echo("*****************************************************************");
    echo("Segment 1 and 2, they should intersect at point (3,2)");
    printSegments(s1, s2);
    testForIntersectionAndPrint2(s1, s2);
    doIntersectionAndPrint2(s1, s2);
    newline();

    echo("Segment 1 and 3, they should intersect at segment (2,2), (4,2)");
    printSegments(s1, s3);
    testForIntersectionAndPrint2(s1, s3);
    doIntersectionAndPrint2(s1, s3);
    newline();

    echo("Segment 1 and 4, they should not intersect");
    printSegments(s1, s4);
    testForIntersectionAndPrint2(s1, s4);
    doIntersectionAndPrint2(s1, s4);
    newline();

    echo("Segment 2 and 5, they should intersect at point (3,2)");
    printSegments(s2, s5);
    testForIntersectionAndPrint2(s2, s5);
    doIntersectionAndPrint2(s2, s5);
    newline();

    echo("Segment 2 and 2, they should intersect at segment (2,1), (4,3)");
    printSegments(s2, s2);
    testForIntersectionAndPrint2(s2, s2);
    doIntersectionAndPrint2(s2, s2);
    newline();

    return 0;
}
