// Class that contains the particle filter 
// Needs the map to be initialized and is updated every time an update comes from a log 

#include "particle_filter.h"


namespace str
{

particle_filter::particle_filter(libconfig::Config &cfg, 
			odom &initial_odom, 
			const map_type& map, 
			Grapher& grapher):grapher_(grapher)
{	
	map_ = map;

 	// grapher_=grapher;
	// grapher_=grapher;

	n_particles_ = cfg.lookup("particles.nParticles");

	// create motion model
	motion_model_=std::make_shared<motion_model>(motion_model(cfg,initial_odom));

	// Create Sensor Model
  sensor_params_.uniformParam = cfg.lookup("sensorModel.uniformParam");
  sensor_params_.decayScale =   cfg.lookup("sensorModel.decayScale");
  sensor_params_.decayRate =    cfg.lookup("sensorModel.decayRate");
  sensor_params_.maxParam =     cfg.lookup("sensorModel.maxParam");
  sensor_params_.rangeSTD =     cfg.lookup("sensorModel.rangeSTD");

  // Create observation Model and build range cache
  observation_model_ = std::make_shared<observation_model>(observation_model(sensor_params_));
  observation_model_->forcePopulateRangeCache(map_);

  // auto get_prob_for_particle_bind = std::bind(observation_model_->getProbForParticle(std::placeholders::_1,std::placeholders::_2,map_,*grapher_));
}

void particle_filter::filter_update_odom(odom& odometry_reading)
{


	//Update the motion model
	std::cout<<"\nUpdate Particles";
	motion_model_->update_odometry(odometry_reading);
	std::cout<<"\nPropagate Particles";
	motion_model_->propagate_particles(particle_set_);

}

void particle_filter::filter_update_laser(laser& laser_reading)
{	
	particles new_particles;

	for (size_t p_idx=0; p_idx < particle_set_.size(); ++p_idx)
	{
		//Update weight of particle based on sensor model

		observation_model_->getProbForParticle(particle_set_[p_idx],laser_reading,map_,grapher_);
	
		// get_prob_for_particle_bind(particle_set_[idx],laser_reading);
	}

	//Resample the particles based on their updated weights
	resample(new_particles);

	//Copy these new particles to your particle set
	particle_set_.clear();
	// particle_set_.copy(new_particles.begin(), new_particles.end());
	particle_set_=new_particles;
}

void particle_filter::resample(particles& new_particles)
{
	//Make sure new particles are empty
	new_particles.clear();	

	
	unsigned int num_draws = particle_set_.size();
	double num_draws_inv = 1.0/num_draws;

	// Generate a random number between 0 and num_draws_inv
	double random_number = (std::rand()/RAND_MAX)*num_draws_inv;

	//First Weight 
	double w  = particle_set_.front().weight;

	// Upper bound for resampling
	double upper_bound = 0;

	unsigned int i= 0;

	for (size_t particle_idx = 0 ; particle_idx < particle_set_.size(); ++particle_idx)
	{
		upper_bound = random_number + (particle_idx+1)*num_draws_inv;
		
		while(upper_bound>w)
		{
			++i;
			w+=particle_set_.at(i).weight;
		}
		new_particles.push_back(particle_set_.at(i));
	}
}

void particle_filter::generate_random_particles()
{
	// Compute free space for inital particle locations
	std::vector<std::pair<int, int>> freeSpace;
	unsigned int width = map_.size_x*map_.resolution;
	unsigned int height = map_.size_y * map_.resolution;
	for (unsigned int i = 0; i < width; ++i)
	{
	  for (unsigned int j = 0; j < height; ++j)
	  {
	    if(map_.prob[i/map_.resolution][j/map_.resolution] == 0){
	        freeSpace.push_back(std::pair<int, int>(i,j));
	    }
	  }
	}

	// Initialize particle set
	for (unsigned int i = 0; i < n_particles_; i++)
	{
	    std::pair<int, int> pt;
	    int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
	    pt = freeSpace[r_pt];
	    str::particle newParticle(pt.first, pt.second, r_pt);
	    particle_set_.push_back(newParticle);
	}
}

}//end namespace str