#ifndef _OSM_H
#define _OSM_H

#include <iostream>
#include <cstdlib>     /* atof */

#include <pqxx/pqxx>
#include "time_track.hpp"
#include <SplitPolicy.h>
#include <FilePolicy.h>
#include <ProcessFile.h>

typedef nxm::ProcessFile<nxm::FileUncompress,nxm::SplitPolicy> processFile;

namespace nxm{

class OSM{
	private:
	    std::string osm_name; //("name:zh");
		pqxx::connection *ptrConn;
		pqxx::result resultSet;

		std::map<std::string,std::string> mapRoad;
		std::map<std::string,std::string> mapCountry;
		std::map<std::string,std::string> mapState;
		std::map<std::string,std::string> mapCity;
		std::map<std::string,std::string> mapValue;

		std::string sql;
        float radius;
        std::string place_id;

		//Level 1 - Country (e.g. China)
		//Level 2 - City (e.g. Hong Kong)
		//Level 3 - District (e.g. Tseung Kwan O)
		//Level 4 - Street level (e.g. Tong Tak Street)

	public:

		OSM():
			osm_name("name:zh"),
			radius(0.0008)
			{

			ptrConn = new pqxx::connection("dbname=nominatim user=osm password=osm hostaddr=127.0.0.1 port=5432");

	        if (!ptrConn->is_open())
	               throw std::string("database connection problem.");

	        mapCountry["country"]="place";			//level 1
			mapState["administrative"]="boundary";	//level 2
			mapCity["suburb"]="place";				//level 3
		    mapCity["village"]="place";
		    mapCity["town"]="place";
		    mapCity["city"]="place";
		    mapCity["hamlet"]="place";
		    mapRoad["highway"]="highway";			//level 4
		    mapRoad["highway"]="tertiary";
		}

		~OSM(){
			ptrConn->disconnect();
		}

		void getAddress(float lat, float lon,std::string &address){

			bool placeIDExist(false);
			mapValue.clear();

			pqxx::nontransaction nType((*ptrConn));

	        for (int i=0; i < 10; ++i){

	        	sql="select place_id,parent_place_id,rank_search from placex WHERE ST_DWithin(ST_SetSRID(ST_Point("+std::to_string(lon)+","+std::to_string(lat)+"),4326), "
	   		         "geometry, "+std::to_string(radius)+") and rank_search != 28 and rank_search >= 26 and (name is not null or housenumber is not null) and"
	   		         " class not in ('waterway','railway','tunnel','bridge') and indexed_status = 0 and "
	   		         "(ST_GeometryType(geometry) not in ('ST_Polygon','ST_MultiPolygon') "
	   		         "OR ST_DWithin(ST_SetSRID(ST_Point("+std::to_string(lon)+","+std::to_string(lat)+"),4326), centroid, "+std::to_string(radius)+")) "
	   		         "ORDER BY ST_distance(ST_SetSRID(ST_Point("+std::to_string(lon)+","+std::to_string(lat)+"),4326), geometry) ASC limit 1";

	        	//std::cout << sql << std::endl;

	        	resultSet = nType.exec(sql);

				if (!resultSet.empty()) {
					for (pqxx::result::const_iterator c = resultSet.begin(); c != resultSet.end(); ++c){
						//cout << '\t' << c[0].as(string()) << '\t' << c[1].as(string()) <<'\t' << c[2].as(string()) <<endl;
						place_id = c[0].as(std::string());
					}
					placeIDExist=true;
					break;
				}

				radius*=2;

	        }// end of for loop

	        if(placeIDExist){

	        	resultSet = nType.exec("select name,class,type from get_addressdata(" + place_id + ") where isaddress order by rank_address asc");

	            if (!resultSet.empty()) {
	            	for (pqxx::result::const_iterator c = resultSet.begin(); c != resultSet.end(); ++c){
	            		parseLocation( c[0].as(std::string()), c[1].as(std::string()), c[2].as(std::string()));
	            	}
	            }

	        }

	        printAddress(address);

		} // end of func getAddress


	private:

		void printAddress(std::string &address){

			splitMap::iterator it;
			//for (it = mapValue.begin(); it != mapValue.end(); ++it)
			//	std::cout << (*it).first << "=>" << (*it).second << std::endl;

			address.clear();
			it = mapValue.find("country");
			if(it != mapValue.end())
				address.append((*it).second);
			else
				address.append("0");

			address.append("ᚖ");

			it = mapValue.find("city");
			if(it != mapValue.end())
				address.append((*it).second);
			else
				address.append("0");

			address.append("ᚖ");

			it = mapValue.find("district");
			if(it != mapValue.end())
				address.append((*it).second);
			else
				address.append("0");

			address.append("ᚖ");

			it = mapValue.find("street");
			if(it != mapValue.end())
				address.append((*it).second);
			else
				address.append("0");
		}

		void parseLocation(const std::string &_name, const std::string &_class, const std::string &_type){

			processFile o;
			splitMap m;
			splitMap::iterator it;
			splitMap::iterator itVal;

			bool flagCR(false);
			bool flagState(false);
			bool flagCity(false);
			bool flagRoad(false);

			it = mapCountry.find( _class );
			if(it != mapCountry.end()){
				flagCR=true;
			}else{
				it = mapCountry.find( _type );
				if(it != mapCountry.end())
					flagCR=true;
			}

			it = mapState.find( _class );
			if(it != mapState.end()){
				flagState=true;
			}else{
				it = mapState.find( _type );
				if(it != mapState.end())
					flagState=true;
			}

			it = mapCity.find( _class );
			if(it != mapCity.end()){
				flagCity=true;
			}else{
				it = mapCity.find( _type );
				if(it != mapCity.end())
					flagCity=true;
			}

			it = mapRoad.find( _class );
			if(it != mapRoad.end()){
				flagRoad=true;
			}else{
				it = mapRoad.find( _type );
				if(it != mapRoad.end())
					flagRoad=true;
			}

			if(flagCR==true){
				o.Split(_name.c_str(),m,", ");
				it = m.find(osm_name);
				if(it != m.end())
				mapValue.insert(std::pair<std::string,std::string>("country",(*it).second));
			else
				mapValue.insert(std::pair<std::string,std::string>("country",m.find("name")->second));

			}

			if(flagState==true){
				o.Split(_name.c_str(),m,", ");
				it = m.find(osm_name);
				if(it != m.end()){
					itVal = mapValue.find("district");
					if(itVal != mapValue.end())
						itVal->second = itVal->second + " : " + (*it).second;
					else
						mapValue.insert(std::pair<std::string,std::string>("district",(*it).second));

				}else{

					itVal = mapValue.find("district");
					if(itVal != mapValue.end())
						itVal->second = itVal->second + " : " + m.find("name")->second;
					else
						mapValue.insert(std::pair<std::string,std::string>("district",m.find("name")->second));
				}

			}

			if(flagCity==true){
				o.Split(_name.c_str(),m,", ");
				it = m.find(osm_name);
				if(it != m.end()){
					itVal = mapValue.find("city");
					if(itVal != mapValue.end())
						itVal->second = itVal->second + " : " + (*it).second;
					else
						mapValue.insert(std::pair<std::string,std::string>("city",(*it).second));

				}else{

					itVal = mapValue.find("city");
					if(itVal != mapValue.end())
						itVal->second = itVal->second + " : " + m.find("name")->second;
					else
						mapValue.insert(std::pair<std::string,std::string>("city",m.find("name")->second));
				}

			}

			if(flagRoad==true){
				o.Split(_name.c_str(),m,", ");
				it = m.find(osm_name);
				if(it != m.end())
				mapValue.insert(std::pair<std::string,std::string>("street",(*it).second));
			else
				mapValue.insert(std::pair<std::string,std::string>("street",m.find("name")->second));

			}

		} //end of parseLocation




	};

};

#endif
