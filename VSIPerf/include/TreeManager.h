#ifndef __COMMONTOOLS_TREEMANAGER__
#define __COMMONTOOLS_TREEMANAGER__

/**
   
   Yet another TTree manager
   
   Author: Hide Oide ( Hideyuki.Oide@cern.ch )
   
   Created: 2018-MAR-11

 */

#include <include/AnyType.h>

#include <TTree.h>

#include <string>
#include <map>

class TFile;

// TTree manager
class TreeManager {
 protected:
  
  std::unique_ptr<TTree> m_tree;
  
  static std::map<std::string, std::map<std::string, anytype> > s_vars;
  
 public:
  
  enum class error { unsupported_type, not_possible_to_branch, collection_not_found };
    
  static bool hasCollection( const std::string name = "main" );
  static void resetCollection( const std::string name = "main" );
  static void clearVars( const std::string name = "main" );
  static std::map<std::string, std::map<std::string, anytype> >& collections();
  static std::map<std::string, anytype>& collection( const std::string name = "main" );
  
  template<class T>
  static void set( std::map<std::string, anytype>& collection, const std::string var_name, const T var );
  
  template<class T>
  static void emplace_back( std::map<std::string, anytype>& collection, const std::string var_name, const T var );
  
  
  TreeManager();
  virtual ~TreeManager();
  
  void initialize(const std::string&,  TFile* );

  template<class T>
  void book( const char* name, T* var );
  
  virtual void book( const char* name, anytype& var );
  void fill();
  
};


template<class T>
void TreeManager::set( std::map<std::string, anytype>& collection, const std::string var_name, const T var ) {
  
  if( collection.find( var_name ) == collection.end() ) {
    
    collection[var_name] = var;
    
  } else {
    
    collection[var_name] = var;
    
  }
  
}
  

template<class T>
void TreeManager::emplace_back( std::map<std::string, anytype>& collection, const std::string var_name, const T var ) {
  
  if( collection.find( var_name ) == collection.end() ) {
    
    collection[var_name] = std::vector<T>{ var };
    
  } else {
    
    collection[var_name].getVar< std::vector<T> >().emplace_back( var );
    
  }
  
}
  

template<class T>
void TreeManager::book( const char* name, T* var ) {
  if( nullptr == m_tree->GetBranch( name ) ) {
    m_tree->Branch( name, var );
  } else {
    auto* branch = m_tree->GetBranch( name );
    m_tree->SetBranchAddress( name, &var, &branch );
  }
}

#endif /* __COMMONTOOLS_TREEMANAGER__ */
