#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //s(x) = (x - c) * (x -c) = r^2
    // r(t) = e + Ut
    Hit hit;
    vec3 W = ray.endpoint - center;
    double a, b, c, t1, t2, root;
    a = ray.direction.magnitude_squared();
    b = 2*dot(ray.direction, W);
    c = W.magnitude_squared() - (radius*radius);
    root = (b*b) - (4*a*c);
    if(root < 0){
        hit = {0, 0, 0};
    }
    else{
        t1 = ((-1)*b + sqrt(root))/(2*a);
        t2 = ((-1)*b - sqrt(root))/(2*a);
        if(t1 < t2 && t1 >= small_t){ //t1 is closer to camera
            hit = {this, t1, 1};
        }
        else if(t2 <= t1 && t2 >= small_t){ //t2 is closer to camera
            hit = {this, t2, 1};
        }
        else{ //no intersect
            hit = {0, 0, 0};
        }
    }    
    return hit;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal = point - center; // compute a vector that is normal to the sphere.
    normal.normalized(); // Creates a unit vector of the normal.
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
