#ifndef DRIFTSOURCE_H
#define DRIFTSOURCE_H

class Driftsource
{
public:
	virtual ~Driftsource();

	virtual double nextValue() = 0;
};

class ConstantDrift: public Driftsource
{
	double drift;

public:
	ConstantDrift(double drift);

	double nextValue();
};

class BoundedDrift: public Driftsource
{


};

#endif
