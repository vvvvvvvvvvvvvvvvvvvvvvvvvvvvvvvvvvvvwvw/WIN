import Object;

export module Random;

export namespace win
{
	class Random : public Object<Random>
	{
	public:

		int Next(int min, int max);
		double NextDouble(double min, double max);
		float NextFloat(float min, float max);
		char NextBytes(char min, char max);

	};
}