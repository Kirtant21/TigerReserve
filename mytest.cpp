// UMBC - CMSC 341 - Fall 2022 - Proj2
#include "streak.h"
#include <vector>
#include <random>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
  Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type){
    if (type == NORMAL){
      //the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
      //the mean and standard deviation can change by passing new values to constructor
      m_normdist = std::normal_distribution<>(mean,stdev);
    }
    else if (type == UNIFORMINT) {
      //the case of UNIFORMINT to generate integer numbers
      // Using a fixed seed value generates always the same sequence
      // of pseudorandom numbers, e.g. reproducing scientific experiments
      // here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10);// 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min,max);
    }
    else{ //the case of UNIFORMREAL to generate real numbers
      m_generator = std::mt19937(10);// 10 is the fixed seed value
      m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
    }
  }
  void setSeed(int seedNum){
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }

  int getRandNum(){
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if(m_type == NORMAL){
      //returns a random number in a set with normal distribution
      //we limit random numbers by the min and max values
      result = m_min - 1;
      while(result < m_min || result > m_max)
	result = m_normdist(m_generator);
    }
    else if (m_type == UNIFORMINT){
      //this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum(){
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result*100.0)/100.0;
    return result;
  }
private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;//normal distribution
  std::uniform_int_distribution<> m_unidist;//integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
public:
  /******************************************
   * Test function declarations go here! *
   ******************************************/
  bool checkAVLtreeBalance(Tiger *tiger); // testing if the AVL tree member vriables are pointing the appropriate values
  bool checkAVLtreeProperty(Tiger *tiger, bool isPreserved = true); //setting isPreserved to true by default
  Tiger* getRoot(Streak &st); // get m_root from the streak
  int checkBalance(Tiger* bTiger);

};

int main(){
  Random idGen(MINID,MAXID);
  Random ageGen(0,2);
  Random genderGen(0,2);
  Tester test1;
  {
    Streak streak;
    int streakSize = 10;
    int tempID = 0;
    int ID = 0;
    for(int i=0;i<streakSize;i++){
      ID = idGen.getRandNum();
      if (i == streakSize / 2) tempID = ID;//we store this ID for later use
      Tiger tiger(ID,
		  static_cast<AGE>(ageGen.getRandNum()),
		  static_cast<GENDER>(genderGen.getRandNum()));
      streak.insert(tiger);
    }
    cout << "\nDump after inserting " << streakSize << " nodes:\n\n";
    streak.dumpTree();
    cout << "\n\nList of Tigers after inserting " << streakSize << " nodes:\n";
    streak.listTigers();
    cout << endl;

    streak.remove(tempID);
    cout << "\nDump after removig the node with ID: " << tempID << "\n\n";
    streak.dumpTree();
    cout << "\n\nList of Tigers after removing the node with ID: " << tempID << "\n";
    streak.listTigers();
    cout << endl;

    //************************ Test functions being called here **************
    
    cout << endl;
    cout << "**************************** Unit Test ***************" << endl;
    cout<<endl;

    // FUNCTIONALITY TEST CASE - CUB COUNT ***************

    Streak testStreak;
    int testStreakIDs[streakSize];
    
    cout << "Test for fuctionality - Count Tiger Cubs" << endl;

    cout << "Tiger Cubs: " << streak.countTigerCubs() << endl; // tests from the main streak tree that was given to us with project files. 

    //EDGE CASE - CUB COUNT ******************************

    //Below edge case creats a tree with all the cubs (10 cubs total) and checks for them

    Streak streakCubCount;

    for(int i=0;i<10;i++){ // adds all the cubs in the tree
      ID = idGen.getRandNum();
      Tiger tiger(ID,CUB,
		  static_cast<GENDER>(genderGen.getRandNum()));
      streakCubCount.insert(tiger);
    }

    cout << "Test for Edge case - COUNT TIGER CUB " << endl;

    cout << "Tiger cubs: " << streakCubCount.countTigerCubs() << endl;
    
    //NORMAL CASE - INSERT ***************************

    //normal insertion is happing below
    
    for(int i=0;i<streakSize;i++){
      ID = idGen.getRandNum();
      testStreakIDs[i] = ID;
      if (i == streakSize / 2) tempID = ID;//we store this ID for later use
      Tiger tiger(ID,
		  static_cast<AGE>(ageGen.getRandNum()),
		  static_cast<GENDER>(genderGen.getRandNum()));
      testStreak.insert(tiger);
    }

    cout << "Test for normal case - INSERT" << endl;
    
    //ERROR CASE - INSERT *******************************

    //below error case enters a value in negative and a value more than MAXID

    cout << "Test for error case - INSERT" << endl;
    
    Tiger tiger(tempID, // tempID already exist - this is an error case
		static_cast<AGE>(ageGen.getRandNum()),
		static_cast<GENDER>(genderGen.getRandNum()));
   
    testStreak.insert(tiger);

    tiger.setID(-11133); // entering a negative number - an error case
    testStreak.insert(tiger);

    tiger.setID(99999999); // entering a bigger number than MAXID - an error case
    testStreak.insert(tiger);
    


    // EDGE CASE - INSERT **************************
    
    Streak streakInsertEdgeCase;
    streakSize = 300; // making the size of 300
    int insertEdgeCaseIDs[streakSize]; // to store all the random ids for later use
    
    for(int i=0;i<streakSize;i++){
      ID = idGen.getRandNum();
      insertEdgeCaseIDs[i] = ID; // storing values for later use
      if (i == streakSize / 2) tempID = ID;//we store this ID for later use
      Tiger tiger(ID,
		  static_cast<AGE>(ageGen.getRandNum()),
		  static_cast<GENDER>(genderGen.getRandNum()));
      streakInsertEdgeCase.insert(tiger);
    }
    cout << "Test for edge case with 300 nodes - INSERT " << endl;

    Tiger* tigerRoot = test1.getRoot(streakInsertEdgeCase);

    // below if statment checks if tree is still balanced after INSERTING 300 nodes
   
    if(test1.checkAVLtreeBalance(tigerRoot))
      {
	cout << "AVL tree is balanced - TEST PASSED!" << endl;
      }
    else{
      cout << "AVL tree is not balanced - TEST FAILED" << endl;
      }

    // below if statment checks if all the properties are still preserved after adding 300 nodes
    
    if(test1.checkAVLtreeProperty(test1.getRoot(streakInsertEdgeCase)))
      {
	cout << "AVL tree insertion properties are preserved - TEST PASSED!" << endl;
      }
    else{
      cout << "AVL tree insertion properties are preserved - TEST FAILED!" << endl;

    }

    
    // NORMAL CASE - REMOVE ********************

    // normal remove case is happening below

    testStreak.remove(testStreakIDs[3]); // removes 3rd id from the tree

    
    if(testStreak.findTiger(testStreakIDs[3]))
      {
	cout << "Test for remove (normal case) - TEST FAILD!" << endl;
      }
    else{
      cout << "Test for remove (normal case) - TEST PASSED!" << endl;
    }

    
    // EDGE CASE - REMOVE *************************

    //below edge case tests by adding only 1 node to the tree and removing it
    
    Streak streakRemove;
    Tiger rTiger(tempID,
		 static_cast<AGE>(ageGen.getRandNum()),
		 static_cast<GENDER>(genderGen.getRandNum()));

    streakRemove.insert(rTiger); // adding only 1 node - edge case
    streakRemove.remove(tempID);
    
    if(streakRemove.findTiger(tempID))
      {
	cout << "Test for remove (edge case) - TEST FAILD!" << endl;
      }
    else{
      cout << "Test for remove (edge case) - TEST PASSED!" << endl;
      } 

    // EDGE CASE - REMOVE (PHASE 2) *******************

    //below edge case tests remove function by removing 150 nodes from the tree

    int checkID[streakSize/2];

    for(int i=0;i<streakSize/2;i++) //removes 150 nodes from the tree with 300 nodes
      {
	checkID[i] = insertEdgeCaseIDs[i]; // for later use
	streakInsertEdgeCase.remove(insertEdgeCaseIDs[i]);
      }

    bool isRemoved = false;
    
    for(int i = 0; i < streakSize/2; i++) // checks if all the appropriate tigers are removed from the tree
      {
	if(streakInsertEdgeCase.findTiger(checkID[i]))
	  {
	    isRemoved = false;
	    i = streakSize/2; //Breaks the loop if a node is not removed from above loop
	  }else{
	  isRemoved = true;
	}
      }

    if(isRemoved) // checks if every node is removed is true
      {
	cout << "Test for remove Edge case (removed 150 nodes)- TEST PASSED!" << endl;
      }
    else{
      cout << "Test for remove Edge case (removed 150 nodes)- TEST FAILD!" << endl;
    }

    Tiger* tRoot = test1.getRoot(streakInsertEdgeCase);

   //below if statment checks if the tree is still balanced after removing 150 nodes

    
    if(test1.checkAVLtreeBalance(tRoot))
      {
	cout << "AVL tree is balanced - TEST PASSED!" << endl;
      }
    else{
      cout << "AVL tree is not balanced - TEST FAILED" << endl;
    }

    // below if statment checks if the property of the tree are still preserved after the removal of 150 nodes
    
    if(test1.checkAVLtreeProperty(test1.getRoot(streakInsertEdgeCase))) 
      {
       cout << "AVL tree insertion properties are preserved - TEST PASSED!" << endl;
      }
    else{
      cout << "AVL tree insertion properties are preserved - TEST FAILED!" << endl;

    }
    
    //FUNCTIONALITY CASE - REMOVE DEAD ******************************

    // checking for functionality
    
    Streak streakRemoveDead;
    int deadID[10];
    for(int i=0;i<10;i++){
      ID = idGen.getRandNum();
      deadID[i] = ID;
      Tiger dTiger(ID,CUB,
		  static_cast<GENDER>(genderGen.getRandNum()));
      streakRemoveDead.insert(dTiger);
    }

    
    streakRemoveDead.setState(test1.getRoot(streakRemoveDead)->getID(), DEAD); // setting m_root to dead
    streakRemoveDead.removeDead();// removes dead root
    streakRemoveDead.setState(deadID[2], DEAD); // setting m_root to dead
    streakRemoveDead.removeDead(); // removes a dead from the middle

    // below if statment checks if all the DEAD tigers are removed or not

   
      
    if(streakRemoveDead.findTiger(test1.getRoot(streakRemoveDead)->getID()) && streakRemoveDead.findTiger(deadID[2]))
      {
	cout << "Test for functionality (removeDead) - TEST FAILD! " << endl;

      }else{
      cout << "Test for functionality (removeDead) - TEST PASSED!" << endl;
    }

    // TIME COMPLEXITY TEST - INSERT **********************

    cout<<endl;
    cout<< "********************************** Time Complexity************" << endl;
    cout<<endl;

    cout << "****** INSERTION **********" << endl;
    cout<<endl;
    
    cout << "Thesis: Insertion is O(LOG N)" << endl;
    cout << endl;
    
    clock_t start, stop;
    Streak streakTimeTest;
    double time = 0.0, time2 = 0.0;

    start = clock(); // clock start to ticks
    
    for(int i = 0; i<1000; i++) // Inserting 1000 nodes
      {
	//ID = idGen.getRandNum();
	Tiger timeTiger(i + 10000,
		    static_cast<AGE>(ageGen.getRandNum()),
		    static_cast<GENDER>(genderGen.getRandNum()));
	streakTimeTest.insert(timeTiger);
      }

    stop = clock(); // clock stops ticking

    time = stop - start;

    cout << "Inserted 1000 nodes. " << endl;
    cout << "insertion time: " << time << endl;

    cout<<endl;
    cout << "second run time: " << endl;

    // checking time for the case when we insert more than 1000 nodes
    
    Streak streakTimeTest2;
    clock_t start2, stop2;
    
    start2 = clock(); // clock start to ticks

    for(int i = 0; i<2000; i++) // Inserting 2000 nodes
      {
	//ID = idGen.getRandNum();
	Tiger timeTiger(i + 10000,
			static_cast<AGE>(ageGen.getRandNum()),
			static_cast<GENDER>(genderGen.getRandNum()));
	streakTimeTest2.insert(timeTiger);
      }

    stop2 = clock(); // clock stops ticking

    time2 = stop2 - start2;

    cout << "Inserted 2000 nodes. " << endl;
    cout << "insertion time: " << time2 << endl;  
    cout<<endl;
    
    time = time*2;

    cout<<endl;
    cout << "Insertion ratio: " << time2/time <<endl;
    cout<<endl;


    // ********* Proof for insertion *********

    
    if(((1.1 - 0.4) < (time2 / time)) < (1.1 + 0.4)) // checks the prove
      {
	cout << "It is peoven that Insertion is O(LOG N)" << endl;
      }
    else{
      cout << "Insertion is not 0(LOG N)" << endl;
      }


    // TIME COMPLEXITY TEST - REMOVE *******************************


    
    cout << endl;
    cout << "******** REMOVE *********" << endl;
    cout << endl;
    
    clock_t startRemove, stopRemove;
    double removeTime = 0.0, removeTime2 = 0.0;

    startRemove = clock(); // clock starts to tick
    
    for(int i = 0; i < 1000; i++) // removing 1000 nodes from the streakTimeTest2
      {
	streakTimeTest.remove(i+10000);
      }

    stopRemove = clock(); // clock stops to tick

    removeTime = stopRemove - startRemove;

    cout << "Removed 1000 nodes. " << endl;
    cout << "Removal time: " << removeTime << endl;
    cout<<endl;


    // checking time for the case when we remove more than 1000 nodes
    clock_t startRemove2, stopRemove2;
    
    startRemove2 = clock(); // clock starts to tick

    for(int i = 0; i < 2000; i++) // removing 2000 nodes from the streakTimeTest2
      {
	streakTimeTest2.remove(i+10000); 
      }

    stopRemove2 = clock(); // clock stops to tick

    removeTime2 = stopRemove2 - startRemove2;
    
    cout << "second run time:" << endl;
    cout << "Removed 2000 nodes" << endl;
    cout << "Removal time: " << removeTime2 << endl;

    removeTime = removeTime*2;

    cout<<endl;
    cout << "Removal ratio: " << removeTime2/removeTime <<endl;
    cout<<endl;


    // ********** Proof for removal ***********

    
    if(((1.1 - 0.4) < (removeTime2 / removeTime)) < (1.1 + 0.4)) // checks the prove
      {
	cout << "It is peoven that removal is O(LOG N)" << endl;
      }
    else{
      cout << "Removal is not 0(LOG N)" << endl;
    }
    
  }
  return 0;
}

bool Tester:: checkAVLtreeBalance(Tiger *root){
  
  Tiger* temp = root;
  bool itWorked = false;
  int rightSide;
  int leftSide;
  int balance = 0;
  
  if(temp != nullptr)
    {
      rightSide = checkBalance(root->m_right);
      leftSide = checkBalance(root->m_left);
      
      balance = abs(leftSide - rightSide);
      
      if((balance <= 1 && checkAVLtreeBalance(root->m_left)) && checkAVLtreeBalance(root->m_right))
	{
	  
	  itWorked = true;
	  return itWorked;
	}
      else{
	itWorked = false;
	return itWorked;
      }
    }
  else{
    return true;
  }
}

bool Tester::checkAVLtreeProperty(Tiger *tiger, bool isPreserved){

  // Note: isPreserved is true by default (in the prototype)
  
  if(tiger == nullptr)
    {
      return true;
    }
    
  checkAVLtreeProperty(tiger->m_left);
  checkAVLtreeProperty(tiger->m_right);

    if(tiger->m_right != nullptr)
      {
	if(tiger->m_id > tiger->m_right->m_id) // if right side is less than current node than it's not preserved - error
	  {
	    isPreserved = false; 
	  }
	
      }
    
    if(tiger->m_left != nullptr)
      {
	if(tiger->m_id < tiger->m_left->m_id) // if left side is greater than current node than it's not preserved - error
	  {
	    isPreserved = false; 
	  }
      }

    return isPreserved;
}


int Tester::checkBalance(Tiger* bTiger){


  if(bTiger == nullptr)
    {
      return 0;
    }
  
  return max(checkBalance(bTiger->m_left), checkBalance(bTiger->m_right));
}

Tiger* Tester:: getRoot(Streak &streak){
  return streak.m_root;  
}

