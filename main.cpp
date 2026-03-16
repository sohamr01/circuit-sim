#include <iostream>
#include <string>
#include <vector>
#include <fstream> //required to read file 

#define MAX_N 10

class Component {
    public: //meaning that everything below can be accessed outside the class. for our purposes, it means that we can use these variables in main() too.
    std::string name; //creates a variable called name. only strings require including the <string> header (std is the namespace qualifier, and string is the class)
    int node1; //stores first node number that the resistor is connected to
    int node2; //stores second node number that the resistor is connected to

    void print() const {
        std::cout << "Component: " << name << std::endl;
    }
};

class Resistor : public Component { //this means that Resistor inherits from Component, public inheritance makes sure everything in Component is available to the class Resistor
    public:
    double resistance; //stores the resistance value

    void print() const { //resistor needs its own print because it should print resistor-specific info. will probably do this for other components too.
    std::cout << "Resistor: " << name << std::endl; //note: std::endl; is rarely used, it inserts a newline and flushes buffer which is kinda slow. in most cases u can js use \n
    std::cout << "Node1: " << node1 << std::endl;
    std::cout << "Node2: " << node2 << std::endl;
    std::cout << "Resistance: " << resistance << " ohms" << std::endl;
    }
};

class VoltageSource : public Component {
    public: 
    double voltage;

    void print() const {
        std::cout << "Voltage Source: " << name << std::endl;
        std::cout << "Node1: " << node1 << std::endl;
        std::cout << "Node2: " << node2 << std::endl;
        std::cout << "Voltage: " << voltage << " volts" << std::endl;  
    }
};

class Circuit {
    public: 
    std::vector<Resistor> resistors; //declare a vector named resistors of class Resistor (which means it'll contain Resistor objects)
    std::vector<VoltageSource> VSources;

    void addResistor(Resistor r) {
        resistors.push_back(r); //adds Resistor r to the end of the vector resistors
    }

    void addVoltageSource(VoltageSource V) {
        VSources.push_back(V);
    }

    void printCircuit() const { 
        std::cout << "Circuit contents: \n";
        int i = 0;
        while(i < resistors.size()) {
            resistors[i].print();
            i++;
        }

        int j = 0;
        while (j < VSources.size()) {
            VSources[j].print();
            j++;
        }
    }

    std::vector<int> getNodes() const {
        std::vector<int> nodes;

        int i = 0;
        while (i < resistors.size()) {
            Resistor r = resistors[i];

            bool foundNode1 = false;
            int j = 0;
            while (j < nodes.size()) {
                if (nodes[j] == r.node1) {
                    foundNode1 = true;
                }
                j++;
            }
            if (!foundNode1) {
                nodes.push_back(r.node1);
            }

            bool foundNode2 = false;
            j = 0;
            while (j < nodes.size()) {
                if (nodes[j] == r.node2) {
                    foundNode2 = true;
                }
                j++;
            }
            if (!foundNode2) {
                nodes.push_back(r.node2);
            }

            i++;
        }

        i = 0;
        while (i < VSources.size()) {
            VoltageSource v = VSources[i];

            bool foundNode1 = false;
            int j = 0;
            while (j < nodes.size()) {
                if (nodes[j] == v.node1) {
                    foundNode1 = true;
                }
                j++;
            }
            if (!foundNode1) {
                nodes.push_back(v.node1);
            }

            bool foundNode2 = false;
            j = 0;
            while (j < nodes.size()) {
                if (nodes[j] == v.node2) {
                    foundNode2 = true;
                }
                j++;
            }
            if (!foundNode2) {
                nodes.push_back(v.node2);
            }

            i++;
        }

        return nodes;
    }
    
    void printNodes() const {
        std::vector<int> nodes = getNodes();
        std::cout << "Nodes in circuit:\n";
        int i = 0;
        while (i < nodes.size()) {
            std::cout << nodes[i] << std::endl;
            i++;
        }
    }

    int countNodes() const {
        std::vector<int> nodelist = getNodes();
        int count = 0;
        int i = 0;
        while (i < nodelist.size()) {
            if (nodelist[i] != 0) {
                count++;
            }
            i++;
        }
        return count;
    }

    void printNodeMap() const {
        std::vector<int> nodelist = getNodes();
        int currentIndex = 0;
        int i = 0;
        while (i < nodelist.size()) {
            if(nodelist[i] != 0) {
                std::cout << "node " << nodelist[i] << " --> " << "index " << currentIndex << std::endl;
                currentIndex++;
            }
            i++;
        }
    }

    int getNodeIndex(int targetNode) const {
        std::vector<int> nodelist = getNodes();
        int currentIndex = 0;
        int i = 0;
        while (i < nodelist.size()) {
            if(nodelist[i] != 0) {
                if(nodelist[i] == targetNode) {
                    return currentIndex;
                }
                currentIndex++;
            }
            i++;
        }
        return -1;
    }

    std::vector<int> getUnknownNodes() const {
        std::vector<int> nodelist = getNodes();
        std::vector<int> unknownNodes;
        int i = 0;
        while (i < nodelist.size()) {
            if (nodelist[i] != 0 && !isKnownVoltageNode(nodelist[i])) {
                unknownNodes.push_back(nodelist[i]);
            }
            i++;
        }
        return unknownNodes;
    }

    int getUnknownNodeIndex(int targetNode) const {
        std::vector<int> unknownNodes = getUnknownNodes();
        int i = 0;
        while (i < unknownNodes.size()) {
                if(unknownNodes[i] == targetNode) {
                    return i;
                }
            i++;
        }
        return -1;
    }

    //checks if a node is a non-ground terminal of a voltage source (to GND)
    bool isKnownVoltageNode(int targetNode) const {
        int i = 0;
        while (i < VSources.size()) {
            VoltageSource v = VSources[i];
            if ((v.node1 == targetNode && v.node2 == 0) || (v.node2 == targetNode && v.node1 == 0)) {
                return true;
            }
            i++;
        }
        return false;
    }
    //this function returns the fixed voltage at the known node
    double getKnownVoltage (int targetNode) const {
        int i = 0;
        while (i < VSources.size()) {
            VoltageSource v = VSources[i];

            if(v.node1 == targetNode && v.node2 == 0) {
                return v.voltage;
            }
            else if(v.node2 == targetNode && v.node1 == 0) {
                return -v.voltage;
            }
            i++;
        }
        return 0.0;
    }
};

/*
void stampResistors1x1(Circuit c, double V1, double G[1][1], double rhs[1]) { //returntype void because its job is just to modify G and rhs, not solve for anything
    G[0][0] = 0.0;
    rhs[0] = 0.0;
    
    int i = 0; 
    while (i < c.resistors.size()) {
        Resistor r = c.resistors[i];
        double g = 1.0 / r.resistance;

        if ((r.node1 == 1 && r.node2 == 2) || (r.node1 == 2 && r.node2 == 1)) {
            G[0][0] += g;
            rhs[0] += g * V1;
        }
        else if ((r.node1 == 2 && r.node2 == 0) || (r.node1 == 0 && r.node2 == 2)) {
            G[0][0] += g;
        }

        i++;

    }
}

double solve1x1(double G[1][1], double rhs[1]) { //the 1's appearing in the declaration because we need a 1x1 matrix or a matrix with 1 element
    return rhs[0] / G[0][0]; //G[0][0] is to ACCESS the element cos indexing starts at 0, 0x0 is the only valid element in the matrix.
}
*/

void stampResistors(const Circuit& c, double G[MAX_N][MAX_N], double rhs[MAX_N], int n) {
    int i = 0;
    while (i <= n-1) {
        rhs[i] = 0.0;
        int j = 0;
        while (j <= n-1) {
            G[i][j] = 0.0;
            j++;
        }
        i++;
    }

    i = 0;
    while (i < c.resistors.size()) {
        Resistor r = c.resistors[i];
        double g = 1.0 / r.resistance;

        int index1 = c.getUnknownNodeIndex(r.node1);
        int index2 = c.getUnknownNodeIndex(r.node2);

        //case 1: node1 unknown and node2 known
        if (index1 != -1 && index2 == -1) {
            G[index1][index1] += g;
            if (c.isKnownVoltageNode(r.node2)) {
                rhs[index1] += g * c.getKnownVoltage(r.node2);
            }
        }
        //case 2: both ends unknown
        else if (index1 != -1 && index2 != -1) {
            G[index1][index1] += g;
            G[index2][index2] += g;
            G[index2][index1] -= g;
            G[index1][index2] -= g;
        }
        //case 3: node 1 known, node 2 unknown
        else if (index1 == -1 && index2 != -1) {
            G[index2][index2] += g;
            if (c.isKnownVoltageNode(r.node1)) {
                rhs[index2] += g * c.getKnownVoltage(r.node1);
            }
        }

        i++;
    }
}

/*
void solve2x2(double G[2][2], double rhs[2], double x[2]) {
    double det = G[0][0] * G[1][1] - G[0][1] * G[1][0]; 
    x[0] = (rhs[0] * G[1][1] - G[0][1] * rhs[1]) / det;
    x[1] = (G[0][0] * rhs[1] - rhs[0] * G[1][0]) / det;
}
*/

void solveGaussian(double G[MAX_N][MAX_N], double rhs[MAX_N], double x[MAX_N], int n) {
    int pivotRow = 0;
    while (pivotRow < n) {
        int row = pivotRow + 1;
        while (row < n) {
            double factor = G[row][pivotRow] / G[pivotRow][pivotRow];

            int col = pivotRow;
            while (col < n) {
                G[row][col] = G[row][col] - factor * G[pivotRow][col];
                col++;
            }

            rhs[row] = rhs[row] - factor * rhs[pivotRow];
            row++;
        }
        pivotRow++;
    }

    int row = n - 1;
    while (row >= 0) {
        double sum = 0.0;

        int col = row + 1;
        while (col < n) {
            sum = sum + G[row][col] * x[col];
            col++;
        }

        x[row] = (rhs[row] - sum) / G[row][row];
        row--;
    }
}

void readNetlist(std::string filename, Circuit& c) { //&c refers to the address of c, so it modifies the actual circuit
    std::ifstream infile(filename); //input file stream in file: (name of file)
    if (!infile) { //checks if the file opened correctly. if false, display error
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    //following variables store netlist fields:
    std::string name;
    int node1;
    int node2;
    double value;
    
    while (infile >> name >> node1 >> node2 >> value) {
        if (name[0] == 'R') {
            Resistor r;
            r.name = name;
            r.node1 = node1;
            r.node2 = node2;
            r.resistance = value;
            c.addResistor(r);
        }
        else if (name[0] == 'V') {
            VoltageSource v;
            v.name = name;
            v.node1 = node1;
            v.node2 = node2;
            v.voltage = value;
            c.addVoltageSource(v);
        }
    }
}


int main() {

    Circuit c;
/*
    //assigning object r1 of class Resistor various properties assigned in class Resistor
    Resistor r1;
    r1.name = "R1";
    r1.node1 = 1; 
    r1.node2 = 2;
    r1.resistance = 1000.0;

    Resistor r2;
    r2.name = "R2";
    r2.node1 = 2; 
    r2.node2 = 3;
    r2.resistance = 2000.0;

    Resistor r3;
    r3.name = "R3";
    r3.node1 = 3;
    r3.node2 = 0;
    r3.resistance = 1000.0;

    VoltageSource v1;
    v1.name = "V1";
    v1.node1 = 1;
    v1.node2 = 0;
    v1.voltage = 5.0;

    c.addResistor(r1);
    c.addResistor(r2);
    c.addResistor(r3);
    c.addVoltageSource(v1);
*/
    
    readNetlist("example.cir", c);
    c.printCircuit();
    c.printNodes();
    c.printNodeMap();

    std::cout << "Index of node 1: " << c.getNodeIndex(1) << std::endl;
    std::cout << "Index of node 2: " << c.getNodeIndex(2) << std::endl;
    std::cout << "Index of node 0: " << c.getNodeIndex(0) << std::endl;

    if(c.VSources.size() == 0) {
        std::cout << "Error - no voltage source found in circuit" << std::endl;
        return 1; //signifies program terminating due to error 
    }


    //syntax of matrix: name[row][col]. so G[0][0] is row 0 col 0 in matrix G. 
    double G[MAX_N][MAX_N]; //G = conductance matrix
    double rhs[MAX_N];
    double x[MAX_N]; //voltage we're solving for 

    int n = c.getUnknownNodes().size(); //number of unknown nodes in the circuit 

    stampResistors(c, G, rhs, n);


    std::cout << "G matrix before solve:\n";
    int r = 0;
    while (r < n) {
        int col = 0;
        while (col < n) {
            std::cout << G[r][col] << " ";
            col++;
        }
        std::cout << std::endl;
        r++;
    }

    std::cout << "rhs vector before solve:\n";
    r = 0;
    while (r < n) {
        std::cout << rhs[r] << std::endl;
        r++;
    }

    solveGaussian(G, rhs, x, n);

    std::vector<int> unknownNodes = c.getUnknownNodes();
    int i = 0;
    while (i < unknownNodes.size()) {
        std::cout << "V(node " << unknownNodes[i] << ") = " << x[i] << " volts" << std::endl;
        i++;
    }

    return 0;
}