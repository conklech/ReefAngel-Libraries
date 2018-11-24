#ifndef __RA_SERVICES_H__
#define __RA_SERVICES_H__

/*
 * Service design principles
 *
 * - Everything can be done in the sketch/main.cpp. It is never mandatory to modify some
 *   header.
 * - No use of conditional compilation.
 * - Minimal memory overhead.
 * - Consistent developer experience.
 * - Understanding of relatively few "advanced" C++11 concepts required: constexpr and
 *   templates.
 * - ServiceManager class is implemented with variadic templates, which
 *   end-users/developers should not be required to understand; good samples should be
 *   provided for copy-pasting.
 *
 * General pattern
 *
 * - Each service has a main -Service class that is immutable; normally defined as
 *   constexpr.
 * - Each service also likely has an associated -Variables class that has no public
 *   fields.
 * - Services may also provide (constexpr) interface objects to simplify external API.
 * 
 * The main service class must implement these members:
 * 
 * - void Setup();
 * - void Run();
 * 
 *
 */


#endif // __RA_SERVICES_H__