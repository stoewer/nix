#include <pandora/Group.hpp>
#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <pandora/SectionTreeIterator.hpp>
#include <pandora/Util.hpp>
#include <pandora/Property.hpp>
#include <pandora/PropertyIterator.hpp>
#include <iostream>

using namespace std;

namespace pandora {

Section::Section(const Section &section) :
				NamedEntity(section.group,section.entity_id), file(section.file){
	property_group = section.property_group;
	section_group = section.section_group;
}

Section::Section(File file, Group group,const  string &id) :
				NamedEntity(group,id), file(file){
	property_group = group.openGroup("properties");
	section_group = group.openGroup("sections");
}

Section::Section(File file, Group group, const string &id, time_t time) :
				NamedEntity(group,id,time),file(file)
{
	property_group = group.openGroup("properties");
	section_group = group.openGroup("sections");
}

void Section::repository(const string &repository) {
	group.setAttr("repository", repository);
}

string Section::repository() const {
	string repository;
	group.getAttr("repository", repository);
	return repository;
}

void Section::link(const string &link) {
	vector<Section> sects = this->file.findSection(link);
	if(sects.size() == 1 && sects[0].type().compare(this->type()) == 0) {
		group.setAttr("link", link);
	}
	else if (sects.empty()) {
		throw std::runtime_error(
				"Cannot create link! Linked section does not exist!");
	}
	else {
		throw std::runtime_error(
				"Cannot create link to a section of deviating type!");
	}
}

string Section::link() const {
	string link;
	group.getAttr("link", link);
	return link;
}


void Section::mapping(const string &mapping) {
	group.setAttr("mapping", mapping);
}

string Section::mapping() const {
	string mapping;
	group.getAttr("mapping", mapping);
	return mapping;
}

void Section::parent(const string &parent) {
	group.setAttr("parent", parent);
}

string Section::parent() const {
	string parent;
	group.getAttr("parent", parent);
	return parent;
}

bool Section::hasParent() const{
	return (parent().length() != 0 && !file.findSection(parent()).empty());
}

Section Section::findParent() const{
	std::vector<Section> s = file.findSection(parent());
	if(s.empty())
		throw std::runtime_error(
				"Section has no parent or parent could not be found!");
	return file.findSection(parent())[0];
}

Section Section::addSection(const std::string &name, const std::string &type) {
	string id = util::createId("section");
	Section s(file, section_group.openGroup(id,true), id);
	s.name(name);
	s.type(type);
	s.parent(this->id());
	return s;
}

bool Section::removeSection(const std::string &id) {
	bool success = false;
	if(section_group.hasGroup(id)){
		section_group.removeGroup(id);
		success = true;
	}
	return success;
}

bool Section::hasSection(const std::string &id) const{
	return section_group.hasGroup(id);
}

std::vector<std::string> Section::getRelatedSections(const std::string &type) const{
	std::vector<std::string> victor = findDownstream(type);
	if(victor.size() != 0){
		return victor;
	}
	std::string t = findUpstream(type);
	if(t.length() != 0){
		victor.push_back(t);
		return victor;
	}

	victor = findSideways(type);

	return victor;
}

bool Section::hasRelatedSection(const std::string &type) const{
	std::vector<std::string> victor = findDownstream(type);
	if(victor.size() != 0){
		return true;
	}
	if(findUpstream(type).length() != 0){
		return true;
	}
	return findSideways(type).size() != 0;
}

std::vector<std::string> Section::findDownstream(const std::string &type) const{
	std::vector<std::string> victor;
	SectionTreeIterator iter = this->treeIterator(type,0);
	while (iter != iter.end()){
		victor.push_back((*iter).id());
		++iter;
	}
	return victor;
}

std::string Section::findUpstream(const std::string &type) const{
	std::string id;
	if(hasParent()){
		Section p = findParent();
		if(p.type().compare(type) == 0){
			return p.id();
		}
		id = p.findUpstream(type);
		if(id.length() != 0){
			return id;
		}
	}
	return id;
}

std::vector<std::string> Section::findSideways(const std::string &type) const{
	std::vector<std::string> victor;
	if(hasParent()){
		Section p = findParent();
		SectionTreeIterator iter = p.treeIterator(type, 1);
		while(iter != iter.end()){
			victor.push_back((*iter).id());
			++iter;
		}
		if(victor.size() != 0){
			return victor;
		}
		victor = p.findSideways(type);
		if(victor.size() != 0){
			return victor;
		}
	}
	return victor;
}

std::vector<Section> Section::findSection(const std::string &id)const{
	vector<Section> sects;
	findSectionRec(id, sects);
	return sects;
}

std::vector<Section> Section::sections() const{
	vector<Section>  section_obj;
	size_t section_count = section_group.objectCount();
	for (size_t i = 0; i < section_count; i++) {
		string id = section_group.objectName(i);
		Group g = section_group.openGroup(id,false);
		Section s(file,g,id);
		section_obj.push_back(s);
	}
	return section_obj;
}

void Section::findSectionRec(const std::string &id, vector<Section> &sects) const{
	std::vector<Section> s = sections();
	for (size_t i = 0; i < s.size(); i++){
		if(s[i].id().compare(id) == 0){
			sects.push_back(s[i]);
			return;
		}
	}
	for (size_t i = 0; i < s.size(); i++){
		s[i].findSectionRec(id, sects);
		if(sects.size() > 0){
			return;
		}
	}
}

bool Section::hasChildren() const {
	SectionIterator iter = this->children();
	return iter != iter.end();
}

SectionIterator Section::children(const std::string &type) const {
	SectionIterator iter(file, section_group, type);
	return iter;
}

SectionTreeIterator Section::treeIterator(const std::string &type, uint depth) const {
	SectionTreeIterator iter(*this, type, depth);
	return iter;
}

size_t Section::childCount() const {
	size_t childCount = 0;
	for (SectionIterator iter = this->children(); iter != iter.end(); ++iter) {
		childCount++;
	}
	return childCount;
}

PropertyIterator Section::properties() const {
	PropertyIterator iter(*this, property_group);
	return iter;
}


PropertyIterator Section::inheritedProperties() const {
	if(this->link().length() > 0){
		std::vector<Section> sects = file.findSection(this->link());
		if(!sects.empty()){
			return sects[0].properties();
		}
	}
	throw std::runtime_error(
			"Section has no link, cannot retrieve inherited Properties!");
}

Property Section::getProperty(const std::string &id) const {
	if (property_group.hasGroup(id)) {
		return Property(*this, property_group.openGroup(id, false), id);
	} else {
		throw std::runtime_error(
				"Requested Property does not exist! Always check with hasProperty!");
	}
}

Property Section::getPropertyByName(const std::string &name) const {
	for (PropertyIterator iter = properties(); iter != iter.end(); ++iter) {
		Property p = *iter;
		if (p.name() == name)
			return p;
	}
	if (this->link().length() > 0) {
		std::vector<Section> s = this->file.findSection(this->link());
		if (!s.empty()) {
			if (s[0].hasPropertyByName(name)) {
				return s[0].getPropertyByName(name);
			}
		}
	}
	throw std::runtime_error(
			"Requested Property does not exist! Always check with hasPropertyByName!");
}

Property Section::addProperty(const std::string &name) {
	if (hasPropertyByName(name)) {
		throw std::runtime_error("Attempt to add a property that already exists!");
	}
	string new_id = util::createId("property");
	while (property_group.hasObject(new_id))
		new_id = util::createId("property");
	Property p(*this, property_group.openGroup(new_id, true), new_id);
	p.name(name);
	return p;
}

void Section::removeProperty(const std::string &id) {
	if (property_group.hasObject(id)) {
		property_group.removeGroup(id);
	}
}

size_t Section::propertyCount() const {
	size_t count = property_group.objectCount();
	if (group.hasGroup("values"))
		count--;
	return count;
}

bool Section::operator==(const Section &other) const {
	return entity_id == other.entity_id;
}

bool Section::operator!=(const Section &other) const {
	return entity_id != other.entity_id;
}

bool Section::hasProperty(const std::string &id) const {
	return property_group.hasGroup(id);
}

bool Section::hasPropertyByName(const string &name) const {
	for (PropertyIterator iter = properties(); iter != iter.end(); ++iter) {
		Property p = *iter;
		if (p.name() == name)
			return true;
	}
	return false;
}

Section::~Section() {
	//dtor
}

} // end namespace pandora
