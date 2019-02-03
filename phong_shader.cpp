#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>
#include <cmath>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    vec3 l;
    vec3 r;
    vec3 shadow;
    color = world.ambient_color*world.ambient_intensity*color_ambient;
    if(world.enable_shadows){
        for (Light* light : world.lights){
            l = light->position - intersection_point; //vector from object to the light source
            Ray shadowRay(intersection_point, l); //shadow ray for decting objects between light source and the intersetion point
            Hit shadowHit = world.Closest_Intersection(shadowRay);
            if(shadowHit.dist > 0){
                shadow = shadowRay.Point(shadowHit.dist) - intersection_point;
            }
            if(!shadowHit.object || shadow.magnitude() > l.magnitude()){
                color += color_diffuse*light->Emitted_Light(l)*(std::max(dot(normal,l.normalized()), 0.0));
                r = -l + 2*(dot(l, normal))*normal;
                color += color_specular*light->Emitted_Light(l)*std::pow(std::max(dot(-ray.direction, r.normalized()), 0.0), specular_power);
            } 
        }
    }
    else{
        for(Light* light : world.lights){
            l = light->position - intersection_point;
            color += color_diffuse*light->Emitted_Light(l)*(std::max(dot(normal,l.normalized()), 0.0));
            r = -l + 2*(dot(l, normal))*normal;
            color += color_specular*light->Emitted_Light(l)*std::pow(std::max(dot(-ray.direction, r.normalized()), 0.0), specular_power);
        }
    }
    return color;
}
