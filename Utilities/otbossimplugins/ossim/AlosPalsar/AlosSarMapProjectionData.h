#ifndef AlosSarMapProjectionData_h
#define AlosSarMapProjectionData_h

#include <iostream>
#include <cstdlib>
#include <AlosSarRecordHeader.h>
#include <AlosSarRecord.h>
/**
 * @ingroup AlosSarMapProjectionDataRecord
 * @brief This class is able to read the SAR leader data set summary record of the leader file
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class AlosSarMapProjectionData : public AlosSarRecord
{
public:
	/**
	 * @brief Constructor
	 */
	AlosSarMapProjectionData();

	/**
	 * @brief Destructor
	 */
	~AlosSarMapProjectionData();

	/**
	 * @brief This function write the AlosSarMapProjectionData in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const AlosSarMapProjectionData& data);

	/**
	 * @brief This function read a AlosSarMapProjectionData from a stream
	 */
	friend std::istream& operator>>(std::istream& is, AlosSarMapProjectionData& data);

	/**
	 * @brief Copy constructor
	 */
	AlosSarMapProjectionData(const AlosSarMapProjectionData& rhs);

	/**
	 * @brief Copy operator
	 */
	AlosSarMapProjectionData& operator=(const AlosSarMapProjectionData& rhs);

	/**
	 * @brief This function is able to create a new instance of the class
	 */
	AlosSarRecord* Instanciate()
	{
		return new AlosSarMapProjectionData();
	};

	/**
	 * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
	 */
	AlosSarRecord* Clone()
	{
		return new AlosSarMapProjectionData(*this);
	};

	/**
	 * @brief Read the class data from a stream
	 */
	void Read(std::istream& is)
	{
		is>>*this;
	};

	/**
	 * @brief Write the class to a stream
	 */
	void Write(std::ostream& os)
	{
		os<<*this;
	};

	/**
	* @brief map_proj_des
	*/
	std::string   get_map_proj_des()
	{
		return _map_proj_des;
	};
	/**
	* @brief num_pix_in_line
	*/
	int   get_num_pix_in_line()
	{
		return _num_pix_in_line;
	};
	/**
	* @brief num_lines
	*/
	int   get_num_lines()
	{
		return _num_lines;
	};
	/**
	* @brief nom_interpixel_dist
	*/
	double   get_nom_interpixel_dist()
	{
		return _nom_interpixel_dist;
	};
	/**
	* @brief nom_interline_dist
	*/
	double   get_nom_interline_dist()
	{
		return _nom_interline_dist;
	};
	/**
	* @brief orientation_at_center
	*/
	double   get_orientation_at_center()
	{
		return _orientation_at_center;
	};
	/**
	* @brief orbit_incl
	*/
	double   get_orbit_incl()
	{
		return _orbit_incl;
	};
	/**
	* @brief asc_node_long
	*/
	double   get_asc_node_long()
	{
		return _asc_node_long;
	};
	/**
	* @brief platform_heading
	*/
	double   get_platform_heading()
	{
		return _platform_heading;
	};
	/**
	* @brief name_of_ref_ellipsoid
	*/
	std::string   get_name_of_ref_ellipsoid()
	{
		return _name_of_ref_ellipsoid;
	};
	/**
	* @brief semi_maj_axis
	*/
	double   get_semi_maj_axis()
	{
		return _semi_maj_axis;
	};
	/**
	* @brief semi_min_axis
	*/
	double   get_semi_min_axis()
	{
		return _semi_min_axis;
	};
	/**
	* @brief first_line_first_pixel	_lat
	*/
	double   get_first_line_first_pixel_lat()
	{
		return _first_line_first_pixel_lat;
	};
	/**
	* @brief first_line_first_pixel_lon
	*/
	double   get_first_line_first_pixel_lon()
	{
		return _first_line_first_pixel_lon;
	};
	/**
	* @brief first_line_last_pixel	_lat
	*/
	double   get_first_line_last_pixel_lat()
	{
		return _first_line_last_pixel_lat;
	};
	/**
	* @brief first_line_last_pixel_lon
	*/
	double   get_first_line_last_pixel_lon()
	{
		return _first_line_last_pixel_lon;
	};
	/**
	* @brief last_line_last_pixel_lat
	*/
	double   get_last_line_last_pixel_lat()
	{
		return _last_line_last_pixel_lat;
	};
	/**
	* @brief last_line_last_pixel_lon
	*/
	double   get_last_line_last_pixel_lon()
	{
		return _last_line_last_pixel_lon;
	};
	/**
	* @brief last_line_first_pixel_lat
	*/
	double   get_last_line_first_pixel_lat()
	{
		return _last_line_first_pixel_lat;
	};
	/**
	* @brief last_line_first_pixel_lon
	*/
	double   get_last_line_first_pixel_lon()
	{
		return _last_line_first_pixel_lon;
	};

protected:
	/**
	* @brief map_proj_des
	*/
	std::string   _map_proj_des;
	/**
	* @brief num_pix_in_line
	*/
	int   _num_pix_in_line;
	/**
	* @brief num_lines
	*/
	int   _num_lines;
	/**
	* @brief nom_interpixel_dist
	*/
	double   _nom_interpixel_dist;
	/**
	* @brief nom_interline_dist
	*/
	double   _nom_interline_dist;
	/**
	* @brief orientation_at_center
	*/
	double   _orientation_at_center;
	/**
	* @brief orbit_incl
	*/
	double   _orbit_incl;
	/**
	* @brief asc_node_long
	*/
	double   _asc_node_long;
	/**
	* @brief platform_heading
	*/
	double   _platform_heading;
	/**
	* @brief name_of_ref_ellipsoid
	*/
	std::string   _name_of_ref_ellipsoid;
	/**
	* @brief semi_maj_axis
	*/
	double   _semi_maj_axis;
	/**
	* @brief semi_min_axis
	*/
	double   _semi_min_axis;
	/**
	* @brief first_line_first_pixel_lat
	*/
	double   _first_line_first_pixel_lat;
	/**
	* @brief first_line_first_pixel_lon
	*/
	double   _first_line_first_pixel_lon;
	/**
	* @brief first_line_last_pixel_lat
	*/
	double   _first_line_last_pixel_lat;
	/**
	* @brief first_line_last_pixel_lon
	*/
	double   _first_line_last_pixel_lon;
	/**
	* @brief last_line_last_pixel_lat
	*/
	double   _last_line_last_pixel_lat;
	/**
	* @brief last_line_last_pixel_lon
	*/
	double   _last_line_last_pixel_lon;
	/**
	* @brief last_line_first_pixel_lat
	*/
	double   _last_line_first_pixel_lat;
	/**
	* @brief last_line_first_pixel_lon
	*/
	double   _last_line_first_pixel_lon;


private:

};

#endif