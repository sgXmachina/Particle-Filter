#include <iostream>
#include "grapher.h"
#include "helper_functions.h"

str::Grapher::Grapher(int gridSize, int windowSize):
	sfMapArray(sf::Quads, 4*gridSize*gridSize),
	sfWindow(sf::VideoMode(windowSize, windowSize), "Map")
{
	m_windowSize = windowSize;
	m_gridSize = gridSize;
	m_cellSize = m_windowSize / m_gridSize;

	std::cout << "Window is set to " << m_windowSize
		<< " and cell size is " << m_cellSize << "\n";
	
	m_radius = 2;
	sfCentroid.setRadius(m_cellSize*m_radius);
	sfCentroid.setPointCount(4); // Cosmetic, number of edges of circle
	sfCentroid.setFillColor(sf::Color::Green);
		// sf::VertexArray lArray(sf::Lines, 180*2);

}

str::Grapher::Grapher(int gridSize, int windowSize, int sensorWindowSize):
	sfMapArray(sf::Quads, 4*gridSize*gridSize),
	sfWindow(sf::VideoMode(windowSize, windowSize), "Map"),
	sfSensorWindow(sf::VideoMode(sensorWindowSize, sensorWindowSize), "Sensor")
{
	m_windowSize = windowSize;
	m_gridSize = gridSize;
	m_cellSize = m_windowSize / m_gridSize;

	std::cout << "Window is set to " << m_windowSize
		<< " and cell size is " << m_cellSize << "\n";
	
	m_radius = 2;
	sfCentroid.setRadius(m_cellSize*m_radius);
	sfCentroid.setPointCount(4); // Cosmetic, number of edges of circle
	sfCentroid.setFillColor(sf::Color::Green);
		// sf::VertexArray lArray(sf::Lines, 180*2);

	m_sensorWindowSize = sensorWindowSize;

}


bool str::Grapher::setMap(float** map)
{
	for(int i = 0; i < m_gridSize; i++){
		for(int j = 0; j < m_gridSize; j++){

			sf::Vertex* quad = &sfMapArray[(i + j * m_gridSize) * 4];

			quad[0].position = sf::Vector2f(i*m_cellSize, j*m_cellSize);
			quad[1].position = sf::Vector2f((i+1)*m_cellSize, j*m_cellSize);
			quad[2].position = sf::Vector2f((i+1)*m_cellSize, (j+1)*m_cellSize);
			quad[3].position = sf::Vector2f((i)*m_cellSize, (j+1)*m_cellSize);

	  	int p = (map[i][j]+ 1.0) *255.0/2.0;
	  	quad[0].color = sf::Color(p,p,p);
	  	quad[1].color = sf::Color(p,p,p);
	  	quad[2].color = sf::Color(p,p,p);
			quad[3].color = sf::Color(p,p,p);
		}	
	}
	return true;
}

bool str::Grapher::setParticlePoints(
	const str::particles& particles)
{
	sfParticleArray.clear();
	sfParticleArray.reserve(particles.size());
	double xsum, ysum = 0;
	for(int i = 0; i < particles.size(); i++)
	{
		sf::Vertex pt;
		pt.position = sf::Vector2f(particles[i].x_cm, particles[i].y_cm );
		pt.color = sf::Color::Red;
		sfParticleArray.push_back(pt);
		xsum += particles[i].x_cm;
		ysum += particles[i].y_cm;
	}
}

bool str::Grapher::setLaserLines(
	std::vector<int>& ranges, 
	int xc, int yc)
{
	std::vector<std::pair<double,double>> xy;
	xy = range2Point(ranges);
	sfLaserArray.clear();
	sfLaserArray.reserve(180*2);
	for(int i = 0; i < 180; i++)
	{
		int rx = xy[i].first/100;
		int ry = xy[i].second/100;
		sf::Vertex vert;
		vert.position = sf::Vector2f(xc,yc);
		vert.color = sf::Color::Blue;
		sfLaserArray.push_back(vert);
		vert.position = sf::Vector2f(xc+rx,yc+ry); 
		sfLaserArray.push_back(vert);

	}
	return true;
}

void str::Grapher::setCentroid(int xc, int yc)
{
	sfCentroid.setPosition(sf::Vector2f(xc, yc) );
}

void str::Grapher::updateGraphics()
{
	sfWindow.clear();
  sfWindow.draw(sfMapArray);
  sfWindow.draw(&sfParticleArray[0],sfParticleArray.size(), sf::Points);
  sfWindow.draw(sfCentroid);
  sfWindow.draw(&sfLaserArray[0],sfLaserArray.size(), sf::Lines);
  
  sfWindow.display();
}

bool str::Grapher::setMeasuredRanges(const std::vector<int>& ranges)
{
	sfMeasRanges.clear();
	sfMeasRanges.reserve(ranges.size());

	std::vector<std::pair<double,double>> xy;
	xy = range2Point(ranges);

	double scale = m_sensorWindowSize/800.0 / 5.0; // Sorry for magic scale factor
	std::cout << "Scale factor is " << scale << "\n";
	for(int i = 0; i < ranges.size(); i++)
	{
		sf::Vertex vert;
		vert.position = sf::Vector2f(
			xy[i].first*scale + m_sensorWindowSize/2, 
			xy[i].second*scale + m_sensorWindowSize/4);
		vert.color = sf::Color::Blue;
		sfMeasRanges.push_back(vert);
	}
	return true;
}

bool str::Grapher::setPredictedRanges(const std::vector<int>& ranges)
{

	sfPredRanges.clear();
	sfPredRanges.reserve(ranges.size());

	std::vector<std::pair<double,double>> xy;
	xy = range2Point(ranges);

	double  scale = m_sensorWindowSize/800.0 / 5.0; // Sorry for magic scale factor
	for(int i = 0; i < ranges.size(); i++)
	{
		sf::Vertex vert;
		vert.position = sf::Vector2f(
			xy[i].first*scale + m_sensorWindowSize/2, 
			xy[i].second*scale + m_sensorWindowSize/4);
		vert.color = sf::Color::Red;
		sfPredRanges.push_back(vert);
	}
	return true;
}

void str::Grapher::updateSensorGraphics()
{
	sfSensorWindow.clear();
  sfSensorWindow.draw(&sfPredRanges[0], sfPredRanges.size(), sf::Points);
  sfSensorWindow.draw(&sfMeasRanges[0], sfMeasRanges.size(), sf::Points);
  
  sfSensorWindow.display();
}