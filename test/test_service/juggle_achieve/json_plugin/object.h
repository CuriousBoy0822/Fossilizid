/*
 * object.h
 *
 *  Created on: 2015-1-11
 *      Author: qianqians
 */
#ifndef _json_object_h
#define _json_object_h

#include <boost/make_shared.hpp>

#include "../../juggle/interface/object.h"

#include "../../third_party/json/json_protocol.h"

namespace Fossilizid{
namespace jsonplugin{

class object : public juggle::object{
public:
	object(){
		value = boost::make_shared<Json::Value>();
	}

	object(boost::shared_ptr<Json::Value> v){
		if (v->isArray()){
			for (size_t i = 0; i < v->size(); i++){
				varray.push_back(boost::make_shared<object>((boost::make_shared<Json::Value>((*v)[i]))));
			}
		}else if(v->isObject()){
			for (auto it : v->getMemberNames()){
				vmap.push_back(std::make_pair(it, boost::make_shared<object>((boost::make_shared<Json::Value>((*v)[it])))));
			}
		}else{
			value = v;
		}
	}

	~object(){
		vmap.clear();
		varray.clear();
	}

	/*
	 * object is empty
	 */
	virtual bool empty() const{
		return value->size() == 0;
	}

	/*
	 * clear the object, set object to null
	 */
	virtual void clear() const{
		value->clear();
	}

	/*
	 * get the object value when object is value type
	 */
	virtual bool asbool() const{
		return value->asBool();
	}

	virtual int64_t asint() const{
		return value->asInt64();
	}

	virtual double asfloat() const{
		return value->asFloat();
	}

	virtual std::string asstring() const{
		return value->asString();
	}

	/*
	 * judge the object type
	 */
	virtual bool isnull() const{
		return value->isNull();
	}
	
	virtual bool isbool() const{
		return value->isBool();
	}
	
	virtual bool isint() const{
		return value->isInt64();
	}
	
	virtual bool isfloat() const{
		return value->isDouble();
	}
	
	virtual bool isstring() const{
		return value->isString();
	}
	
	virtual bool isarray() const{
		return value->isArray() || !varray.empty();
	}
	
	virtual bool ismap() const{
		return value->isObject() || !vmap.empty();
	}

	/*
	 * get the object size
	 * effectived when object type is array or map 
	 */
	virtual size_t size() const{
		return value->size();
	}
	
	/*
	 * acess the object as a array
	 */
	virtual juggle::object & operator[](int index){
		return *(varray[index]);
	}
	
	virtual const juggle::object &operator[](int index) const{
		return *(varray[index]);
	}

	
	virtual object & append(const bool & other){
		if (varray.front()->isbool()){
			throw std::exception("type is not bool");
		}
		auto v = boost::make_shared<object>();
		*v = other;
		varray.push_back(v);
		return *this;
	}

	virtual object & append(const int64_t & other){
		if (varray.front()->isint()){
			throw std::exception("type is not int");
		}
		auto v = boost::make_shared<object>();
		*v = other;
		varray.push_back(v);
		return *this;
	}

	virtual object & append(const double & other){
		if (varray.front()->isfloat()){
			throw std::exception("type is not float");
		}
		auto v = boost::make_shared<object>();
		*v = other;
		varray.push_back(v);
		return *this;
	}

	virtual object & append(const std::string & other){
		if (varray.front()->isstring()){
			throw std::exception("type is not string");
		}
		auto v = boost::make_shared<object>();
		*v = other;
		varray.push_back(v);
		return *this;
	}

	virtual bool erase(const int index){
		if (index < (int)value->size()){
			for (size_t i = index; i < value->size() - 1; i++){
				(*value)[i] = (*value)[i + 1];
			}
			return true;
		}

		return false;
	}

	/*
	 * acess the object as a map
	 */
	virtual bool hasfield(const std::string & key) const{
		auto it = vmap.begin();
		for ( ; it != vmap.end(); it++){
			if (it->first == key){
				break;
			}
		}
		return (it != vmap.end()) || value->isMember(key);
	}
	
	virtual object &operator[](const std::string & key){
		auto find = vmap.begin();
		for (; find != vmap.end(); find++){
			if (find->first == key){
				break;
			}
		}

		if (find == vmap.end()){
			vmap.push_back(std::make_pair(key, boost::make_shared<object>()));
			find = vmap.end() - 1;
		}

		return *((*find).second);
	}

	virtual const object &operator[](const std::string & key) const{
		auto find = vmap.begin();
		for (; find != vmap.end(); find++){
			if (find->first == key){
				break;
			}
		}

		if (find == vmap.end()){
			throw std::exception((std::string("has no field") + key).c_str());
		}

		return *((*find).second);
	}

	virtual bool erase(const std::string & key){
		auto find = vmap.begin();
		for (; find != vmap.end(); find++){
			if (find->first == key){
				break;
			}
		}

		if (find == vmap.end()){
			return false;
		}

		return true;
	}

	/*
	 * set value
	 */
	virtual void operator =( const bool & other ){
		*value = other;
	}

	virtual void operator =( const int64_t & other ){
		*value = other;
	}

	virtual void operator =( const double & other ){
		*value = other;
	}

	virtual void operator =( const std::string & other ){
		*value = other;
	}

	virtual void operator =( const char * other ){
		*value = other;
	}

private:
	std::vector<std::pair<std::string, boost::shared_ptr<object> > > vmap;
	std::vector<boost::shared_ptr<object> > varray;

	boost::shared_ptr<Json::Value> value;

	friend boost::shared_ptr<std::string> object_to_buf(juggle::object & value);
	friend inline int buf_to_object(juggle::object & value, char * buf, int len);

};

inline boost::shared_ptr<std::string> object_to_buf(juggle::object & value){
	if (value.isarray()){
		for (size_t i = 0; i < value.size(); i++){
			(((object*)&value)->value)->append(*((object*)&(value[i]))->value);
		}
	}else if (value.ismap()){
		for(auto it :((object*)&value)->vmap){
			(*(((object*)&value)->value))[it.first] = *it.second->value;
		}
	}
	return json_parser::json_to_buf(*(((object*)&value)->value));
}

inline int buf_to_object(juggle::object & value, char * buf, int len){
	boost::shared_ptr<Json::Value> v = boost::make_shared<Json::Value>();
	int unpacklen = json_parser::buf_to_json(*(v), buf, len);
	new (&value) object(v);

	return unpacklen;
}

} /* namespace json */
} /* namespace Fossilizid */

#endif //_object_h