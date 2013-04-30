#ifndef JSONINTERFACE_H
#define JSONINTERFACE_H

class JsonInterface
{
public:
	virtual void get(const char *path, JsonObject *item) = 0;
	virtual void set(const char *path, const char *parameter, JsonObject *item) = 0;
	virtual void add(const char *path, JsonItem *newItem, JsonObject *item) = 0;
protected:
	
private:
};

#endif // JSONINTERFACE_H
