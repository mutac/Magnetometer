#include <cfixcc.h>
#include <string.h>

#include "ResourcePath.h"

class TestResourcePath : public cfixcc::TestFixture
{
private:

public:
	void SetPath()
	{
    ResourcePath path1;
    CFIX_ASSERT (path1.getFullPath() == NULL);
    CFIX_ASSERT (path1.getFullPathLength() == 0);
    CFIX_ASSERT (path1.getPath() == NULL);
    CFIX_ASSERT (path1.getPathLength() == 0);

    ResourcePath path2("system.object");
    CFIX_ASSERT (strcmp(path2.getFullPath(), "system.object") == 0);
    CFIX_ASSERT (path2.getFullPathLength() == 13);
    CFIX_ASSERT (strcmp(path2.getPath(), "system.object") == 0);
    CFIX_ASSERT (path2.getPathLength() == 13);

    ResourcePath path3;
    CFIX_ASSERT (path3.getFullPath() == NULL);
    CFIX_ASSERT (path3.getFullPathLength() == 0);
    CFIX_ASSERT (path3.getPath() == NULL);
    CFIX_ASSERT (path3.getPathLength() == 0);

    path3.setPath("system.core");
    CFIX_ASSERT (strcmp(path3.getFullPath(), "system.core") == 0);
    CFIX_ASSERT (path3.getFullPathLength() == 11);
    CFIX_ASSERT (strcmp(path3.getPath(), "system.core") == 0);
    CFIX_ASSERT (path3.getPathLength() == 11);
	}

  void Relative()
  {
    ResourcePath path1("system.object.item");
    CFIX_ASSERT (strcmp(path1.getPath(), "system.object.item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("system") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "object.item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("system.object") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("system.object.item") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "") == 0);

    ResourcePath root("system");
    CFIX_ASSERT (path1.makeRelativeTo(root) == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "object.item") == 0);

    root.setPath("system.object");
    CFIX_ASSERT (path1.makeRelativeTo(root) == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    root.setPath("system.object.item");
    CFIX_ASSERT (path1.makeRelativeTo("system.object.item") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "") == 0);

    ResourcePath path2("system.devices.temperature");
    CFIX_ASSERT (path2.isChildOf("system") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices.temperature") == true);
    CFIX_ASSERT (path2.isChildOf("objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects.temperature") == false);
  }

  void Matches()
  {
  }

  void Pop()
  {
    ResourcePath path1("system.part1.part2");

    path1.popFront();
    CFIX_ASSERT (strcmp(path1.getPath(), "part1.part2") == 0);
    path1.popFront();
    CFIX_ASSERT (strcmp(path1.getPath(), "part2") == 0);
    path1.popFront();
    CFIX_ASSERT (strcmp(path1.getPath(), "") == 0);
    path1.popFront();
    CFIX_ASSERT (strcmp(path1.getPath(), "") == 0);
  }
};

CFIXCC_BEGIN_CLASS(TestResourcePath)
	CFIXCC_METHOD(SetPath)
  CFIXCC_METHOD(Relative)
  CFIXCC_METHOD(Matches)
  CFIXCC_METHOD(Pop)
CFIXCC_END_CLASS()

