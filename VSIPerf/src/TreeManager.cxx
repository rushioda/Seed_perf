#include <include/TreeManager.h>
#include <TFile.h>


// Static variables / functions

//____________________________________________________________________________________________________
std::map<std::string, std::map<std::string, anytype> > TreeManager::s_vars;

//____________________________________________________________________________________________________
void TreeManager::resetCollection( const std::string name ) {
  if( s_vars.find( name ) == s_vars.end() ) {
    std::cerr << __PRETTY_FUNCTION__ << ": collection " << name << " doesn't exist!";
    throw TreeManager::error::collection_not_found;
  }
  s_vars.at(name).clear();
}

//____________________________________________________________________________________________________
void TreeManager::clearVars( const std::string name ) {
  if( s_vars.find( name ) == s_vars.end() ) {
    std::cerr << __PRETTY_FUNCTION__ << ": collection " << name << " doesn't exist!";
    throw TreeManager::error::collection_not_found;
  }
  auto& collection = s_vars.at( name );
  for( auto& pair : collection ) {
    pair.second.clear();
  }
}

//____________________________________________________________________________________________________
bool TreeManager::hasCollection( const std::string name ) {
  return ( s_vars.find( name ) != s_vars.end() );
}

//____________________________________________________________________________________________________
std::map<std::string, std::map<std::string, anytype> >& TreeManager::collections() {
  return s_vars;
}

//____________________________________________________________________________________________________
std::map<std::string, anytype>& TreeManager::collection( const std::string name ) {
  if( s_vars.find( name ) == s_vars.end() ) {
    std::map<std::string, anytype> tmp;
    s_vars[name] = std::move(tmp);
  }
  return s_vars.at( name );
}


// Instance things

//____________________________________________________________________________________________________
TreeManager::TreeManager()
  : m_tree ( nullptr )
{}


//____________________________________________________________________________________________________
TreeManager::~TreeManager() {
  m_tree.release();
}

//____________________________________________________________________________________________________
void TreeManager::initialize(const std::string& treeName,  TFile* file ) {
  m_tree.reset( new TTree( treeName.c_str(), treeName.c_str() ) );
  m_tree->SetDirectory( file );
}

//____________________________________________________________________________________________________
void TreeManager::book( const char* name, anytype& var ) {
        
  // here, it's needed to book all possible types stored in anytype

  if( nullptr == m_tree->GetBranch( name ) ) {
    
#define BRANCH( TYPE )     ( var.type() == typeid( TYPE ) ) {                               \
      auto& holder = static_cast<VarHolder< TYPE >&>( var.get() );                          \
      auto* branch = m_tree->Branch( name, holder.get()  );                                 \
      if( nullptr == branch ) {                                                             \
        std::cerr << __PRETTY_FUNCTION__                                                    \
                  << ": the type of variable " << name                                      \
                  << "[ " << demangle( typeid(TYPE).name() ) << " ] "                       \
                  << "is not possible to register to tree!"                                 \
                  << std::endl;                                                             \
        throw( TreeManager::error::not_possible_to_branch );                                \
      }                                                                                     \
      anytype tmp( var );                                                                   \
      var = std::numeric_limits<TYPE>::quiet_NaN();                                         \
      for( auto ientry = 0LL; ientry < m_tree->GetEntries(); ++ientry ) { branch->Fill(); } \
      var = *( static_cast<VarHolder< TYPE >&>( tmp.get() ) );                              \
    }
    // end of #define

#define BRANCH_VEC( TYPE )     ( var.type() == typeid( TYPE ) ) {                           \
      auto& holder = static_cast<VarHolder< TYPE >&>( var.get() );                          \
      auto* branch = m_tree->Branch( name, holder.get()  );                                 \
      if( nullptr == branch ) {                                                             \
        std::cerr << __PRETTY_FUNCTION__                                                    \
                  << ": the type of variable " << name                                      \
                  << "[ " << demangle( typeid(TYPE).name() )  << " ] "                      \
                  << "is not possible to register to tree!"                                 \
                  << std::endl;                                                             \
        throw( TreeManager::error::not_possible_to_branch );                                \
      }                                                                                     \
      anytype tmp( var );                                                                   \
      var = TYPE{};                                                                         \
      for( auto ientry = 0LL; ientry < m_tree->GetEntries(); ++ientry ) { branch->Fill(); } \
      var = *( static_cast<VarHolder< TYPE >&>( tmp.get() ) );                              \
    }
    // end of #define

#define BRANCH_UINT32( TYPE )     ( var.type() == typeid( TYPE ) ) {                        \
      auto& holder = static_cast<VarHolder< TYPE >&>( var.get() );                          \
      auto* branch = m_tree->Branch( name, holder.get(), Form("%s/i",name) );               \
      if( nullptr == branch ) {                                                             \
        std::cerr << __PRETTY_FUNCTION__                                                    \
                  << ": the type of variable " << name                                      \
                  << "[ " << demangle( typeid(TYPE).name() ) << " ] "                       \
                  << "is not possible to register to tree!"                                 \
                  << std::endl;                                                             \
        throw( TreeManager::error::not_possible_to_branch );                                \
      }                                                                                     \
      anytype tmp( var );                                                                   \
      var = std::numeric_limits<TYPE>::quiet_NaN();                                         \
      for( auto ientry = 0LL; ientry < m_tree->GetEntries(); ++ientry ) { branch->Fill(); } \
      var = *( static_cast<VarHolder< TYPE >&>( tmp.get() ) );                              \
    }
    // end of #define

#define BRANCH_UINT64( TYPE )     ( var.type() == typeid( TYPE ) ) {                        \
      auto& holder = static_cast<VarHolder< TYPE >&>( var.get() );                          \
      auto* branch = m_tree->Branch( name, holder.get(), Form("%s/l",name) );               \
      if( nullptr == branch ) {                                                             \
        std::cerr << __PRETTY_FUNCTION__                                                    \
                  << ": the type of variable " << name                                      \
                  << "[ " << demangle( typeid(TYPE).name() ) << " ] "                       \
                  << "is not possible to register to tree!"                                 \
                  << std::endl;                                                             \
        throw( TreeManager::error::not_possible_to_branch );                                \
      }                                                                                     \
      anytype tmp( var );                                                                   \
      var = std::numeric_limits<TYPE>::quiet_NaN();                                         \
      for( auto ientry = 0LL; ientry < m_tree->GetEntries(); ++ientry ) { branch->Fill(); } \
      var = *( static_cast<VarHolder< TYPE >&>( tmp.get() ) );                              \
    }
    // end of #define

    if BRANCH( bool )
    else if BRANCH( char )
    else if BRANCH( short )
    else if BRANCH( int )
    else if BRANCH( long )
    else if BRANCH( long long )
    else if BRANCH( unsigned char )
    else if BRANCH( unsigned short )
    else if BRANCH( unsigned int )
    else if BRANCH_UINT64( unsigned long )
    else if BRANCH( unsigned long long )
    else if BRANCH( float )
    else if BRANCH( double )
    else if BRANCH( uint8_t )
    else if BRANCH( int8_t )
    else if BRANCH( uint16_t )
    else if BRANCH( int16_t )
    else if BRANCH_UINT32( uint32_t )
    else if BRANCH( int32_t )
    else if BRANCH( uint64_t )
    else if BRANCH( int64_t )
    else if BRANCH_VEC( std::vector<uint8_t> )
    else if BRANCH_VEC( std::vector<int8_t> )
    else if BRANCH_VEC( std::vector<uint16_t> )
    else if BRANCH_VEC( std::vector<int16_t> )
    else if BRANCH_VEC( std::vector<uint32_t> )
    else if BRANCH_VEC( std::vector<int32_t> )
    else if BRANCH_VEC( std::vector<uint64_t> )
    else if BRANCH_VEC( std::vector<int64_t> )
    else if BRANCH_VEC( std::vector<unsigned char> )
    else if BRANCH_VEC( std::vector<unsigned short> )
    else if BRANCH_VEC( std::vector<unsigned long> )
    else if BRANCH_VEC( std::vector<unsigned long long> )
    else if BRANCH_VEC( std::vector<char> )
    else if BRANCH_VEC( std::vector<short> )
    else if BRANCH_VEC( std::vector<long> )
    else if BRANCH_VEC( std::vector<long long> )
    else if BRANCH_VEC( std::vector<float> )
    else if BRANCH_VEC( std::vector<double> )
    else if BRANCH_VEC( std::vector<std::vector<uint8_t>> )
    else if BRANCH_VEC( std::vector<std::vector<int8_t>> )
    else if BRANCH_VEC( std::vector<std::vector<uint16_t>> )
    else if BRANCH_VEC( std::vector<std::vector<int16_t>> )
    else if BRANCH_VEC( std::vector<std::vector<uint32_t>> )
    else if BRANCH_VEC( std::vector<std::vector<int32_t>> )
    else if BRANCH_VEC( std::vector<std::vector<uint64_t>> )
    else if BRANCH_VEC( std::vector<std::vector<int64_t>> )
    else if BRANCH_VEC( std::vector<std::vector<unsigned char>> )
    else if BRANCH_VEC( std::vector<std::vector<unsigned short>> )
    else if BRANCH_VEC( std::vector<std::vector<unsigned long>> )
    else if BRANCH_VEC( std::vector<std::vector<unsigned long long>> )
    else if BRANCH_VEC( std::vector<std::vector<char>> )
    else if BRANCH_VEC( std::vector<std::vector<short>> )
    else if BRANCH_VEC( std::vector<std::vector<long>> )
    else if BRANCH_VEC( std::vector<std::vector<long long>> )
    else if BRANCH_VEC( std::vector<std::vector<float>> )
    else if BRANCH_VEC( std::vector<std::vector<double>> )
    else {
      std::cerr << __PRETTY_FUNCTION__ << ": the type of variable " << name << " [ " << demangle( var.type().name() ) << " ] is not supported!" << std::endl;
      throw( TreeManager::error::unsupported_type );
    }
      
#undef BRANCH
#undef BRANCH_VEC
    
    
  }
  
}
    

//____________________________________________________________________________________________________
void TreeManager::fill() { 
#if 0
  auto* branches = m_tree->GetListOfBranches();
  for( int i=0; i< branches->GetEntries(); i++) {
    auto* branch = dynamic_cast<TBranch*>( branches->At(i) );
    auto* address = branch->GetAddress();
    std::cout << branch->GetName() << ": address = " << static_cast<void*>(address) << std::endl;
  }
  m_tree->Print();
#endif
  m_tree->Fill();
}
