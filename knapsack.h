// Knapsack class
// Version f08.1

class knapsack
{
	public:
		knapsack(ifstream &fin);
		knapsack(const knapsack &);
		int getCost(int) const;
		int getValue(int) const;
		int getCost() const;
		int getValue() const;
		int getNumObjects() const;
		int getCostLimit() const;
		void printSolution(bool);
		void select(int);
		void unSelect(int);
		bool isSelected(int) const;
		vector <float> getKnapsackVector(int i); // returns a vector from knapsackMatrix
		float bound();

	private:
		int numObjects;
		int costLimit;
		vector<int> value;
		vector<int> cost;
		vector<bool> selected;
		int totalValue;
		int totalCost;
		vector<float> index; // vector used to store indices of knapsack objects
		vector<float> ratio; // vector used to store value/cost ratios of knapsack objects
		matrix<float> knapsackMatrix; // matrix used to store index and ratio vectors

};

knapsack::knapsack(ifstream &fin)
	// Construct a new knapsack instance using the data in fin.
{
	int n, b, j;
	float v, c; // initialized as floats to obtain value/cost ratio as float

	fin >> n;  // read the number of objects
	fin >> b;  // read the cost limit

	numObjects = n;
	costLimit = b;

	value.resize(n);
	cost.resize(n);
	selected.resize(n);
	index.resize(n); 
	ratio.resize(n);

	for (int i = 0; i < n; i++)
	{
		fin >> j >> v >> c;
		index[j] = j; 
		ratio[j] = v / c; // ratio of knapsack object is value / cost
		value[j] = v;
		cost[j] = c;
		unSelect(j);
	}

	totalValue = 0;
	totalCost = 0;

	knapsackMatrix.fillMatrix(index, ratio); // fill matrix with index and ratio vectors
	knapsackMatrix.sortByRatio(); // sort knapsackMatrix by value/cost ratio of objects
}

knapsack::knapsack(const knapsack &k)
	// Knapsack copy constructor.
{
	int n = k.getNumObjects();

	value.resize(n);
	cost.resize(n);
	selected.resize(n);
	numObjects = k.getNumObjects();
	costLimit = k.getCostLimit();

	totalCost = 0;
	totalValue = 0;

	for (int i = 0; i < n; i++)
	{
		value[i] = k.getValue(i);
		cost[i] = k.getCost(i);
		if (k.isSelected(i))
			select(i);
		else
			unSelect(i);
	}
	knapsackMatrix = k.knapsackMatrix;
}

int knapsack::getNumObjects() const
{
	return numObjects;
}

int knapsack::getCostLimit() const
{
	return costLimit;
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getCost");

	return cost[i];
}

int knapsack::getCost() const
// Return the cost of the selected objects.
{
	return totalCost;
}

int knapsack::getValue() const
// Return the value of the selected objects.
{
	return totalValue;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
	// Print all information about the knapsack.
{
	ostr << "------------------------------------------------" << endl;
	ostr << "Num objects: " << k.getNumObjects() << " Cost Limit: " << k.getCostLimit() << endl;

	int totalValue = 0;
	int totalCost = 0;

	for (int i = 0; i < k.getNumObjects(); i++)
	{
		totalValue += k.getValue(i);
		totalCost += k.getCost(i);
	}

	cout << "Total value: " << totalValue << endl;
	cout << "Total cost: " << totalCost << endl << endl;

	for (int i = 0; i < k.getNumObjects(); i++)
		cout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

	cout << endl;

	return ostr;
}

void knapsack::printSolution(bool file=false)
	// Prints out the solution.
{  
	cout << "------------------------------------------------" << endl;

	cout << "Total value: " << getValue() << endl;
	cout << "Total cost: " << getCost() << endl << endl;

	// Print out objects in the solution
	for (int i = 0; i < getNumObjects(); i++)
		if (isSelected(i))
			cout << i << "  " << getValue(i) << " " << getCost(i) << endl;

	cout << endl;

	if(file)
	{
		ofstream output;
		output.open("knapsack" + to_string(getNumObjects()) + ".output");
		output << "Best solution" << endl;
		output << "------------------------------------------------" << endl;
		output << "Total value: " << getValue() << endl;
		output << "Total cost: " << getCost() << endl << endl;
		for (int i = 0; i < getNumObjects(); i++){
			if (isSelected(i)) {
				output << i << "  " << getValue(i) << " " << getCost(i) << endl;
			}
		}
		output.close();
	}

}

ostream &operator<<(ostream &ostr, vector<bool> v)
	// Overloaded output operator for vectors.
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;

	return ostr;
}

void knapsack::select(int i)
	// Select object i.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::Select");

	if (selected[i] == false)
	{
		selected[i] = true;
		totalCost = totalCost + getCost(i);
		totalValue = totalValue + getValue(i);
	}
}

void knapsack::unSelect(int i)
	// unSelect object i.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::unSelect");

	if (selected[i] == true)
	{
		selected[i] = false;
		totalCost = totalCost - getCost(i);
		totalValue = totalValue - getValue(i);
	}
}

bool knapsack::isSelected(int i) const
// Return true if object i is currently selected, and false otherwise.
{
	if (i < 0 || i >= getNumObjects())
		throw rangeError("Bad value in knapsack::getValue");

	return selected[i];
}

// function that returns a vector containing the index and value/cost ratio
// for a knapsack object
vector <float> knapsack::getKnapsackVector(int i) {
	return knapsackMatrix[i];
}


/*
 * Finds an optimistic LP bound on a knapsack instance by relaxing the integral
 * constraint on selecting items. Sorts items by value/cost ratio, and includes
 * first items that will fit, as well as part of the first item that doesn't fit.
 * The optimal solution to the knapsack instance is guaranteed to be worse than or equal to this bound.
 */
float knapsack::bound() 
{
	// Placeholder
	auto num = getNumObjects();
	auto c = getCost();
	auto v = getValue();

	for (auto i = 0; i < num; ++i) {
		auto vec = getKnapsackVector(i);
		auto idx = vec[0];
		auto ratio = vec[1];

		if(isSelected(idx))
		{
			continue;
		}
		else if(c + getCost(idx) <= getCostLimit())
		{
			c += getCost(idx);
			v += getValue(idx);
		}
		else
		{
			float frac = (float)(getCostLimit() - c) / (float)getCost(idx);
			c += frac * getCost(idx);
			v += frac * getValue(idx);
			return v;
		}
	}
}
