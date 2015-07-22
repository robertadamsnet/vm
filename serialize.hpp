#ifndef serialize_hpp_2015_0703_2219
#define serialize_hpp_2015_0703_2219 

#include "object.hpp"
#include "document.hpp"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>

BOOST_CLASS_VERSION(Object, 0)
BOOST_CLASS_VERSION(Document, 0)

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, Document& doc, const unsigned int version);

template<class Archive>
void serialize(Archive& ar, Object& obj, const unsigned int version);


template<class Archive>
void serialize(Archive& ar, Document& doc, const unsigned int version) {
  ar & make_nvp("license",    doc.data.license);
  ar & make_nvp("author",     doc.data.author);
  ar & make_nvp("copyright",  doc.data.copyright);
  ar & make_nvp("warranty",   doc.data.warranty);
  ar & make_nvp("base", boost::serialization::base_object<Object>(doc));
}

  
template<class Archive>
void serialize(Archive& ar, Object& obj, const unsigned int version)
{
  ar & make_nvp("name", obj.data_.name);
  ar & make_nvp("type", obj.data_.type);
  ar & make_nvp("tags", obj.data_.tags);
  ar & make_nvp("members", obj.data_.members);
}


}
}

#endif//serialize_hpp_2015_0703_2219
