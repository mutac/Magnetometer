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
    CFIX_ASSERT (path1.getAbsolutePath() == NULL);
    CFIX_ASSERT (path1.getAbsolutePathLength() == 0);
    CFIX_ASSERT (path1.getPath() == NULL);
    CFIX_ASSERT (path1.getPathLength() == 0);

    ResourcePath path2("system.object");
    CFIX_ASSERT (strcmp(path2.getAbsolutePath(), "system.object") == 0);
    CFIX_ASSERT (path2.getAbsolutePathLength() == 13);
    CFIX_ASSERT (strcmp(path2.getPath(), "system.object") == 0);
    CFIX_ASSERT (path2.getPathLength() == 13);

    ResourcePath path3;
    CFIX_ASSERT (path3.getAbsolutePath() == NULL);
    CFIX_ASSERT (path3.getAbsolutePathLength() == 0);
    CFIX_ASSERT (path3.getPath() == NULL);
    CFIX_ASSERT (path3.getPathLength() == 0);

    path3.setPath("system.core");
    CFIX_ASSERT (strcmp(path3.getAbsolutePath(), "system.core") == 0);
    CFIX_ASSERT (path3.getAbsolutePathLength() == 11);
    CFIX_ASSERT (strcmp(path3.getPath(), "system.core") == 0);
    CFIX_ASSERT (path3.getPathLength() == 11);
  }

  void ChildOf()
  {
    ResourcePath path2("system.devices.temperature");
    CFIX_ASSERT (path2.isChildOf("system") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices.temperature") == false);
    CFIX_ASSERT (path2.isChildOf("objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects.temperature") == false);
    CFIX_ASSERT (path2.isChildOf("devices") == false);
    CFIX_ASSERT (path2.isChildOf("temperature") == false);

    ResourcePath parent1("system");
    CFIX_ASSERT (path2.isChildOf(parent1) == true);

    ResourcePath parent2("system.devices");
    CFIX_ASSERT (path2.isChildOf(parent2) == true);

    //
    // Check behavior with relative/absolute paths
    // 
    ResourcePath relative("all.life.is.cyclic");

    CFIX_ASSERT (relative.makeRelativeTo("all") == true);
    CFIX_ASSERT (relative.isChildOf("life") == true);
    CFIX_ASSERT (relative.isChildOf("all.life", true) == true);
    CFIX_ASSERT (relative.makeRelativeTo("life") == true);
    CFIX_ASSERT (relative.isChildOf("is") == true);
    CFIX_ASSERT (relative.isChildOf("all.life.is", true) == true);
    CFIX_ASSERT (relative.makeRelativeTo("is") == true);
    CFIX_ASSERT (relative.isChildOf("cyclic") == false);
    CFIX_ASSERT (relative.isChildOf("all.life.is.cyclic", true) == false);
  }

  void Relative()
  {
    ResourcePath path1("system.object.item");
    CFIX_ASSERT (strcmp(path1.getPath(), "system.object.item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("system") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "object.item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("object") == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    CFIX_ASSERT (path1.makeRelativeTo("item") == false);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    // Reset relative path
    path1.resetPath();
    CFIX_ASSERT (strcmp(path1.getPath(), "system.object.item") == 0);

    ResourcePath root("system");
    CFIX_ASSERT (path1.makeRelativeTo(root) == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "object.item") == 0);

    root.setPath("object");
    CFIX_ASSERT (path1.makeRelativeTo(root) == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    root.setPath("item");
    CFIX_ASSERT (path1.makeRelativeTo(root) == false);
    CFIX_ASSERT (strcmp(path1.getPath(), "item") == 0);

    //
    // Check if trying to set to a non-parent
    //

    root.setPath("this.is.a.non.parent");
    CFIX_ASSERT (path1.makeRelativeTo(root) == false);
  }

  void Matches()
  {
    ResourcePath path1("system.object.item.blah");
    CFIX_ASSERT (path1.matches("system.object.item.blah") == true);

    ResourcePath path2("system.object.item.blah");
    CFIX_ASSERT (path1.matches(path2) == true);

    //
    // Modify relative path, and try matching
    //

    path1.makeRelativeTo("system");
    CFIX_ASSERT (path1.matches(path2, true) == true);
    CFIX_ASSERT (path1.matches(path2, false) == false);

    path2.makeRelativeTo("system");
    CFIX_ASSERT (path1.matches(path2, true) == true);
    CFIX_ASSERT (path1.matches(path2, false) == true);

    path2.makeRelativeTo("object");
    CFIX_ASSERT (path1.matches(path2, true) == true);
    CFIX_ASSERT (path1.matches(path2, false) == false);
  }

  void Pop()
  {
    ResourcePath path1("system.part1.part2");

    CFIX_ASSERT (path1.popFront() == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "part1.part2") == 0);
    CFIX_ASSERT (path1.popFront() == true);
    CFIX_ASSERT (strcmp(path1.getPath(), "part2") == 0);
    CFIX_ASSERT (path1.popFront() == false);
    CFIX_ASSERT (strcmp(path1.getPath(), "part2") == 0);

    // Make sure a pop is recoverable
    path1.resetPath();
    CFIX_ASSERT (strcmp(path1.getPath(), "system.part1.part2") == 0);
  }
};

CFIXCC_BEGIN_CLASS(TestResourcePath)
	CFIXCC_METHOD(SetPath)
  CFIXCC_METHOD(ChildOf)
  CFIXCC_METHOD(Relative)
  CFIXCC_METHOD(Matches)
  CFIXCC_METHOD(Pop)
CFIXCC_END_CLASS()

