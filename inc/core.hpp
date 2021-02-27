/**
 * @file core.hpp
 * @brief base include for everything, does not need to be included manually
 * 
 */

/**
 * @brief General Project Namespace
 * 
 */
namespace WhipseeySaveManager
{
	/**
	 * @brief Contains basic Types used in WhipseeySaveManager,
	 * include types.hpp to use, already included in other includes
	 * 
	 */
	namespace Types { }
	
	/**
	 * @brief contains objects/functions that represent/interacts with ini files,
	 * include ini.hpp to use
	 * 
	 */
	namespace INI { }
	
	/**
	 * @brief contains functions that interact with the OS,
	 * include system.hpp to use
	 * 
	 */
	namespace System { }
	
	/**
	 * @brief contains the gui class,
	 * include gui.hpp to use
	 * 
	 */
	namespace GUI { }
	
	/**
	 * @brief contains functions used in testing,
	 * include testhelper.hpp to use
	 * 
	 */
	namespace Test { }
}
