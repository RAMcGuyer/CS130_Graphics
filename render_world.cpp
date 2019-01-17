#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <limits>

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    int min_t = std::numeric_limits<int>::max();
    Hit hit;
    Hit closest_hit = {0, 0 ,0};
    for(Object* o: Render_World::objects){
        hit = o->Intersection(ray, 1);
        if(hit.dist < min_t && hit.dist > small_t){
            min_t = hit.dist;
	    closest_hit = hit; 
        }
    } 
    return closest_hit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
     // set up the initial view ray here
    vec3 end_point = camera.position; // Start point of our ray
    vec3 dir = camera.World_Position(pixel_index) - end_point; //direction vector, I - E
    Ray ray(end_point, dir); // ray constructor already normalizes direction
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit closest_hit = Render_World::Closest_Intersection(ray);
    if(closest_hit.object != 0){ // If hit.object is not null, there is an intersection.
        vec3 ip = ray.Point(closest_hit.dist); //intersection point of ray and object
	vec3 norm = closest_hit.object->Normal(ip, 1); //the vector normal to the surface of the object
        color = closest_hit.object->material_shader->Shade_Surface(ray, ip, norm, recursion_depth); 
    }
    else{
        //back_ground shader is a flat shader, any vectors will do for arguments.
        color = background_shader->Shade_Surface(ray, ray.direction, ray.direction, recursion_depth);
    }    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
