#include <cfixcc.h>
#include <string.h>

#include "PathName.h"

class TestPathName : public cfixcc::TestFixture
{
private:

public:
  void SetPath()
  {
    PathName path1;
    CFIX_ASSERT (path1.getAbsolutePath() == NULL);
    CFIX_ASSERT (path1.getAbsolutePathLength() == 0);
    CFIX_ASSERT (path1.getPath() == NULL);
    CFIX_ASSERT (path1.getPathLength() == 0);

    PathName path2("system.object");
    CFIX_ASSERT (strcmp(path2.getAbsolutePath(), "system.object") == 0);
    CFIX_ASSERT (path2.getAbsolutePathLength() == 13);
    CFIX_ASSERT (strcmp(path2.getPath(), "system.object") == 0);
    CFIX_ASSERT (path2.getPathLength() == 13);

    PathName path3;
    CFIX_ASSERT (path3.getAbsolutePath() == NULL);
    CFIX_ASSERT (path3.getAbsolutePathLength() == 0);
    CFIX_ASSERT (path3.getPath() == NULL);
    CFIX_ASSERT (path3.getPathLength() == 0);

    path3.setPath("system.core");
    CFIX_ASSERT (strcmp(path3.getAbsolutePath(), "system.core") == 0);
    CFIX_ASSERT (path3.getAbsolutePathLength() == 11);
    CFIX_ASSERT (strcmp(path3.getPath(), "system.core") == 0);
    CFIX_ASSERT (path3.getPathLength() == 11);

    PathName path4 = "its.all.the.same.shit";
    CFIX_ASSERT (strcmp(path4.getAbsolutePath(), "its.all.the.same.shit") == 0);
    CFIX_ASSERT (path4.getAbsolutePathLength() == 21);
    CFIX_ASSERT (strcmp(path4.getPath(), "its.all.the.same.shit") == 0);
    CFIX_ASSERT (path4.getPathLength() == 21);

    path4 = "but.they.call.it.clouddead";
    CFIX_ASSERT (strcmp(path4.getAbsolutePath(), "but.they.call.it.clouddead") == 0);
    CFIX_ASSERT (path4.getAbsolutePathLength() == 26);
    CFIX_ASSERT (strcmp(path4.getPath(), "but.they.call.it.clouddead") == 0);
    CFIX_ASSERT (path4.getPathLength() == 26);
  }

  void Relative()
  {
    PathName path1("system.object.item");
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

    PathName root("system");
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

  void Pop()
  {
    PathName path1("system.part1.part2");

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

  void Comparison()
  {
    PathName path1("system.object.item.blah");
    CFIX_ASSERT (path1.compare("system.object.item.blah") == PathName::kMatches);
    CFIX_ASSERT (path1.matches("system.object.item.blah"));
    CFIX_ASSERT (path1 == "system.object.item.blah");


    PathName path2("system.object.item.blah");
    CFIX_ASSERT (path1.compare(path2) == PathName::kMatches);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);

    //
    // Modify relative path, and try matching
    //

    path1.makeRelativeTo("system");
    CFIX_ASSERT (path1.compare(path2, true) == PathName::kMatches);
    // These check the relative path only, so they should fail
    CFIX_ASSERT (path1.compare(path2, false) == PathName::kUnequal);
    CFIX_ASSERT (!path1.matches(path2));
    CFIX_ASSERT (path1 != path2);

    path2.makeRelativeTo("system");
    CFIX_ASSERT (path1.compare(path2, true) == PathName::kMatches);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    CFIX_ASSERT (path1.compare(path2, false) == PathName::kMatches);

    path2.makeRelativeTo("object");
    CFIX_ASSERT (path1.compare(path2, true) == PathName::kMatches);
    // These check the relative path only, so they should fail
    CFIX_ASSERT (path1.compare(path2, false) == PathName::kUnequal);
    CFIX_ASSERT (!path1.matches(path2));
    CFIX_ASSERT (path1 != path2);

    PathName parent("system.devices.test1");
    PathName child("system.devices.test1.somethingnew");
    CFIX_ASSERT (parent != child);

    parent = "system.devices";
    child = "system.devices.device1";
    CFIX_ASSERT (child.compare(parent) == PathName::kIsChild);
    CFIX_ASSERT (parent.compare(child) == PathName::kIsParent);
  }

  void ChildOf()
  {
    PathName path2("system.devices.temperature");
    CFIX_ASSERT (path2.isChildOf("system") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices") == true);
    CFIX_ASSERT (path2.isChildOf("system.devices.temperature") == false);
    CFIX_ASSERT (path2.isChildOf("objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects") == false);
    CFIX_ASSERT (path2.isChildOf("system.objects.temperature") == false);
    CFIX_ASSERT (path2.isChildOf("devices") == false);
    CFIX_ASSERT (path2.isChildOf("temperature") == false);

    PathName parent1("system");
    CFIX_ASSERT (path2.isChildOf(parent1) == true);

    PathName parent2("system.devices");
    CFIX_ASSERT (path2.isChildOf(parent2) == true);

    //
    // Check behavior with relative/absolute paths
    // 
    PathName relative("all.life.is.cyclic");

    CFIX_ASSERT (relative.makeRelativeTo("all") == true);
    CFIX_ASSERT (relative.isChildOf("life") == true);
    CFIX_ASSERT (relative.compare("all.life", true) == PathName::kIsChild);
    //CFIX_ASSERT (ResourcePath("all.life").isParentOf(relative) == true);
    CFIX_ASSERT (PathName("all.life").compare(relative, true) == PathName::kIsParent);
    CFIX_ASSERT (relative.makeRelativeTo("life") == true);
    CFIX_ASSERT (relative.isChildOf("is") == true);
    CFIX_ASSERT (relative.compare("all.life.is", true) == PathName::kIsChild);
    //CFIX_ASSERT (ResourcePath("all.life.is").isParentOf(relative) == true);
    CFIX_ASSERT (PathName("all.life.is").compare(relative, true) == PathName::kIsParent);
    CFIX_ASSERT (relative.makeRelativeTo("is") == true);
    CFIX_ASSERT (relative.isChildOf("cyclic") == false);
    CFIX_ASSERT (relative.compare("all.life.is.cyclic", true) == PathName::kMatches);
  }

  void WildCard() 
  {
    bool matchedWild = false;
    PathName path1("follow.in.your.footsteps");
    PathName path2("");

    path2.setPath("follow.in.your.*");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    path2.setPath("follow.in.*.footsteps");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    path2.setPath("follow.*.your.footsteps");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    path2.setPath("*.in.your.footsteps");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);

    path2.setPath("follow.in.*.*");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    path2.setPath("*.*.your.footsteps");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);
    path2.setPath("*.*.*.*");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == true);
    CFIX_ASSERT (path1.matches(path2));
    CFIX_ASSERT (path1 == path2);

    path2.setPath("follow.in.your.footsteps");
    CFIX_ASSERT (path1.compare(path2, false, &matchedWild) == PathName::kMatches);
    CFIX_ASSERT (matchedWild == false);
  }
};

CFIXCC_BEGIN_CLASS(TestPathName)
	CFIXCC_METHOD(SetPath)
  CFIXCC_METHOD(Relative)
  CFIXCC_METHOD(Pop)
  CFIXCC_METHOD(Comparison)
  CFIXCC_METHOD(ChildOf)
  CFIXCC_METHOD(WildCard)
CFIXCC_END_CLASS()

