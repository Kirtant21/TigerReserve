// UMBC - CMSC 341 - Fall 2022 - Proj2
// Author: Kirtan Thakkar 
#include "streak.h"

Streak::Streak(){

  m_root = nullptr;
}

Streak::~Streak(){

  clear();
  
}

void Streak::insert(const Tiger& tiger){

  // checks for the valid entry
  
  if((tiger.m_id >= MINID && tiger.m_id <= MAXID) && !(findTiger(tiger.m_id)))
    {
      m_root = recursiveTigerInsert(m_root, tiger.m_id, tiger);
    }else{
    cout << "invalid input, please check tiger's id" << endl;
  }

}

void Streak::clear(){

  if(m_root != nullptr)
    {
      destructRecursively(m_root);
    }else{

    cout << "tree is clear" << endl;
  }
  
}

void Streak::remove(int id){
  
  if(m_root != nullptr)
    {
      if(findTiger(id))
	{	  
	  if(id != m_root->m_id) // if node being removed is NOT a m_root node 
	    {	      
	      removeRecursively(m_root, id);
	      m_root = rebalance(m_root);
	      updateHeight(m_root);
	    }else{ //if the node that is being removed is a m_root node then do this

	      removeRoot(); // only removes root - helper function
	    }
	}else{

	cout << "Invalid tiger id - unable to remove node" << endl;
      }

    }else{

    cout << "Tree does not exist!" << endl;
  }
}

void Streak::updateHeight(Tiger* aTiger){
 
  int rightSide = 0;
  int leftSide = 0;
  
  if((aTiger != nullptr) && (aTiger->m_id >= MINID && aTiger->m_id <= MAXID))
    {
      if(aTiger->m_right != nullptr) // gets height from right child
	{
	  rightSide = aTiger->m_right->m_height;

	}else{
	
	rightSide = -1;
	
      }

      if(aTiger->m_left != nullptr) // gets height from left child
	{ 
	  leftSide = aTiger->m_left->m_height;

	}else{

	  leftSide = -1;  
      }

      if(leftSide > rightSide) // checks and resets the height by +1
	{
	  aTiger->setHeight(leftSide+1);

	}else{

	aTiger->setHeight(rightSide+1);
      }
    }
}

int Streak::checkImbalance(Tiger* aTiger){

  int rightSide = -1;
  int leftSide = -1;
  int balance = 0;

  if(aTiger == nullptr)
    {
      return 0;
    }
  
  if((aTiger != nullptr) && (aTiger->m_id >= MINID && aTiger->m_id <= MAXID))
    {
      if(aTiger->m_left != nullptr) // gets height from right side
	{
	  leftSide = aTiger->m_left->m_height;
	}
      
      if(aTiger->m_right != nullptr) // gets height from left side
	{
	  rightSide = aTiger->m_right->m_height;
	}

      balance = leftSide - rightSide;      
      return balance;
    }
  else{
    
    return -1; 
  }
  
}

Tiger* Streak::rebalance(Tiger* aTiger){

  if((aTiger != nullptr) && (aTiger->m_id >= MINID && aTiger->m_id <= MAXID))
    {

      // rotating to right side
      if((checkImbalance(aTiger) > 1) && (checkImbalance(aTiger->m_left) >= 0))
	{
	  return rightRotation(aTiger);
	}
      // rotating to left side
      
      if((checkImbalance(aTiger) < -1) && (checkImbalance(aTiger->m_right) <= 0))
	{
	  return leftRotation(aTiger);
	}
      else if((checkImbalance(aTiger) > 1) && (checkImbalance(aTiger->m_left) <= 0)){  // rotating left then right

	aTiger->m_left = leftRotation(aTiger->m_left);
	
	return rightRotation(aTiger);
	}
      else if((checkImbalance(aTiger) < -1) && (checkImbalance(aTiger->m_right) >= 0)){
	aTiger->m_right = rightRotation(aTiger->m_right);
	return leftRotation(aTiger);
      }
      
    }else{
    return aTiger;
  }
  return aTiger;
}

void Streak::dumpTree() const {
  dump(m_root);

}

void Streak::dump(Tiger* aTiger) const{
  if (aTiger != nullptr){
    cout << "(";
    dump(aTiger->m_left);//first visit the left child
     cout << aTiger->m_id << ":" << aTiger->m_height;//second visit the node itself
    dump(aTiger->m_right);//third visit the right child
    cout << ")";
  }
}

void Streak::listTigers() const {

  if(m_root == nullptr)
    {
      return;
    }
  listTigersRecursively(m_root);
}

bool Streak::setState(int id, STATE state){

  if(findTiger(id))
    {
      Tiger* tempTiger = m_root;

      while(tempTiger != nullptr)
	{
	  if(id == tempTiger->m_id) // checks if id is found or not
	    {
	      tempTiger->setState(state);
	      return true;
	    }
	  else if(id < tempTiger->m_id) // goes right to search for id
	    {
	      tempTiger = tempTiger->m_left;
	    }
	  else if(id > tempTiger->m_id){ // goes left to search for id
	    tempTiger=tempTiger->m_right;
	    
	  }
	}
    }
  
  return false;
}

void Streak::removeDead(){

  if(m_root != nullptr)
    {
      removeDeadRecursively(m_root);
      removeDeadRecursively(m_root);
    }else{

    cout << "No tiger found to be removed, tree is empty!" << endl;
  }
  
}

bool Streak::findTiger(int id) const {

  if(id >= MINID && id <= MAXID)
    {
      Tiger* tTiger = m_root;
      
      if(m_root != nullptr)
	{
	  while(tTiger != nullptr) // goes through entire tree
	    {
	      if(id == tTiger->m_id) // id found
		{
		  return true;
		}
	      else if(id < tTiger->m_id) //goes left
		{
		  tTiger = tTiger->m_left;
		}
	      else if(id > tTiger->m_id) // goes right
		{
		  tTiger = tTiger->m_right;
		}
	      
	    }
	  return false; // if id not found returns false
	  
	}else {
	return false; // if root = nullptr returns false
      }
    }else{
    
    return false; // if ID is invalid returns false
  }
}

int Streak::countTigerCubs() const{
  Tiger* temp = m_root;

  return countCubRecursively(temp); // calls helper function
  
}

Tiger* Streak::recursiveTigerInsert(Tiger* pTiger, int tigerId, const Tiger& nTiger){
 
  if(pTiger != nullptr) 
    {
      if(tigerId < pTiger->getID())// If id is less than current pointer node go to the left side
	{
	  pTiger->setLeft(recursiveTigerInsert(pTiger->getLeft(), tigerId, nTiger));

	  updateHeight(pTiger);
	  pTiger = rebalance(pTiger);
	}
      else if(tigerId > pTiger->getID()){ // insert on right if tigerId is greater than current tiger pointer
	pTiger->setRight(recursiveTigerInsert(pTiger->getRight(), tigerId, nTiger));

	updateHeight(pTiger);
	pTiger = rebalance(pTiger);
	
      }
      
      updateHeight(pTiger); // updates height
      return rebalance(pTiger); // rebalances tree  
    }
  else{

    Tiger* newTiger = new Tiger(tigerId, nTiger.getAge(), nTiger.getGender(), nTiger.getState());

    return newTiger;
  }
}

void Streak::destructRecursively(Tiger* tiger){

  if((tiger != nullptr) && findTiger(tiger->m_id))
    {
      // deletes right and left side recursively
      
      destructRecursively(tiger->m_left);
      destructRecursively(tiger->m_right);
      delete tiger;
      tiger = nullptr;
    }
  else{
    return;
  }

}

Tiger* Streak::removeRecursively(Tiger* root, int id){

  if(id >= MINID && id <= MAXID)
    {
      if(root == nullptr)
	{
	  return root;
	}
      else if(root->m_id < id) // goes right
	{
	  root->m_right = removeRecursively(root->m_right, id);
	}
      else if(root->m_id > id) // goes left
	{
	  root->m_left = removeRecursively(root->m_left, id);
	}
      
      if(root->m_id == id) // if id found
	{
	  Tiger* temp = root;	  
	  if(root->m_left == nullptr)
	    {
	      if(findTiger(root->m_id))
		{
		  root = root->m_right;
		  delete temp;
		  updateHeight(root);
		  root = rebalance(root);
		}
	      return root;
	      
	    }
	  else if(root->m_right == nullptr)
	    {
	      if(findTiger(root->m_id))
		{
		  root = root->m_left;
		  delete temp;
		  updateHeight(root);
		  root = rebalance(root);
		}
	      return root;
	    }
	  else if(root->m_right != nullptr && root->m_left != nullptr){
	    // if both children are not nullptr

	    if(findTiger(root->m_id))
	      {		
		Tiger* tigerTemp = findMinRecursively(root->m_right);		
		root->m_id = tigerTemp->m_id;
		root->m_age = tigerTemp->m_age;
		root->m_gender = tigerTemp->m_gender;
		root->m_state = tigerTemp->m_state;
	    
		root->m_right = removeRecursively(root->m_right, tigerTemp->m_id); //deleting duplicate
				
		tigerTemp = nullptr;
		delete tigerTemp;

		
		
	      }
	    updateHeight(root);
	    root = rebalance(root);
	  }
	}
    }
      return root;
    
}

Tiger* Streak::findMinRecursively(Tiger* tiger){

  if(findTiger(tiger->m_id))
    {
      Tiger* lTiger = tiger;
      while(lTiger->m_left != nullptr && lTiger != nullptr)
	{
	  lTiger = lTiger->m_left;
	}
      return lTiger;

    }else{
    return tiger;
  }

}

Tiger* Streak::rightRotation(Tiger* aTiger){

  if(findTiger(aTiger->m_id))
    {
      Tiger* tempA = aTiger->m_left;
      Tiger* tempB = tempA->m_right;
      
      tempA->m_right = aTiger;
      aTiger->m_left = tempB;

      updateHeight(aTiger);
      updateHeight(tempA);
      //rebalance(aTiger);
      return tempA;
    }else{
    return aTiger;
  }
  
}

Tiger* Streak::leftRotation(Tiger* aTiger){

  if(findTiger(aTiger->m_id))
    {
      Tiger* tempA = aTiger->m_right;
      Tiger* tempB = tempA->m_left;
      
      tempA->m_left = aTiger;
      aTiger->m_right = tempB;
      
      updateHeight(aTiger);
      updateHeight(tempA);
      //rebalance(aTiger);
      return tempA;
    }else {
    return aTiger;
  }
}

void Streak::listTigersRecursively(Tiger* tiger) const{

  if((tiger != nullptr) && (tiger->m_id >= MINID && tiger->m_id <= MAXID))
    {
      listTigersRecursively(tiger->m_left);

      cout << tiger->m_id << ":" << tiger->getAgeStr() << ":" << tiger->getGenderStr() << ":" << tiger->getStateStr() << endl;

      listTigersRecursively(tiger->m_right);
    }else{

    return;
  }

  
}
void Streak::removeDeadRecursively(Tiger* tiger){

  if((tiger != nullptr) && tiger->m_id >= MINID && tiger->m_id <= MAXID)
    {

      removeDeadRecursively(tiger->m_left);
      removeDeadRecursively(tiger->m_right);

      if(tiger->getStateStr() == "DEAD")
	{
	  cout << "A " << tiger->getAgeStr() << " old, " << tiger->getGenderStr() << " tiger (" << tiger->m_id << ") is found dead, removing the tiger now..." << endl;

	  remove(tiger->m_id);
	}

    }else{
    return;
  }

}

void Streak::removeRoot(){
  
  if(m_root != nullptr)
    {
      Tiger* temp = m_root;

      if(m_root->m_left == nullptr && m_root->m_right == nullptr)//no children exist
	{ 
	  m_root->m_left = nullptr;
	  m_root->m_right = nullptr;
	  delete m_root;
	  m_root = nullptr;
	  return;
	}
      else if(m_root->m_right == nullptr && m_root->m_left != nullptr){// only left exist
	temp = temp->m_left;
	m_root->m_left = nullptr;

	delete m_root;
	m_root = temp;
	updateHeight(m_root);
	rebalance(m_root);
	return;

      }
      else if(m_root->m_right != nullptr && m_root->m_left == nullptr){ // only right exist
	
	temp = temp->m_right;
	m_root->m_right = nullptr;

	delete m_root;
	m_root = temp;
	updateHeight(m_root);
	rebalance(m_root);
	return;

      }
      else if(m_root->m_right != nullptr && m_root->m_left != nullptr){ // if both children exist
	
	Tiger* tigerTemp = findMinRecursively(m_root->m_right);

	m_root->setID(tigerTemp->m_id);
	m_root->setAge(tigerTemp->m_age);
	m_root->setGender(tigerTemp->m_gender);
	m_root->setState(tigerTemp->m_state);

	removeRecursively(m_root->m_right, tigerTemp->m_id);
	
	updateHeight(m_root);
	m_root = rebalance(m_root);
	return;
      }
    }
}

int Streak::countCubRecursively(const Tiger* tiger, int count) const{

  if(tiger == nullptr)
    {
      return 0;
    }

 if(tiger->getAgeStr() == "CUB")
   {
     count++;
   } 

 return count + (countCubRecursively(tiger->m_left) + (countCubRecursively(tiger->m_right)));

}
