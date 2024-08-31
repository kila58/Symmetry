/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

// Library namespace (takes 3 ending curlies)
#define NAMESPACE_LIB(_namespace_) namespace Lib { namespace _namespace_ {\

// Core namespace (takes 3 ending curlies)
#define NAMESPACE_CORE(_namespace_) namespace Core { namespace _namespace_ {\

// Bare lib namespace for classes (takes 2 ending curlies)
#define NAMESPACE_LIB_B namespace Lib {\

// Bare core namespace for classes (takes 2 ending curlies)
#define NAMESPACE_CORE_B namespace Core {\
	
// Namespace endings (just curly brackets)
#define NAMESPACE_END1 }
#define NAMESPACE_END2 }}