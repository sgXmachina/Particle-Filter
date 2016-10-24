// Class that contains the particle filter 
// Needs the map to be initialized and is updated every time an update comes from a log 


#ifndef __PARTICLE_FILTER__
#define __PARTICLE_FILTER__

#include "str_io.h"
#include "helper_functions.h"
#include "particle.h"
#include "str_motion_model.h"
#include "str_sensor_model.h"
#include "bee-map.h"

#include <cstdlib>
#include <memory>
#include <libconfig.h++>


namespace str
{
class particle_filter
{
public:
	particle_filter(libconfig::Config &cfg,odom &inital_odom);
	void filter_update(odom odometry_reading, laser laser_reading);

private:

	//Low variance importance resampling of particles 
	void resample(particles& new_particles);
	
	unsigned int number_of_particles;

	std::shared_ptr<motion_model> motion_model_;

	// sensor_model sensor_model_;

	particles particle_set_;

	map_type map_;

};

}// end namespace str

#endif