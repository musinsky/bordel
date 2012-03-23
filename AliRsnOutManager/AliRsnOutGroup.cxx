// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    23 Mar 2012

#include <TList.h>

#include "AliRsnOutGroup.h"

ClassImp(AliRsnOutGroup)

//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup()
: TNamed(),
  fFileName(0),
  fList(0)
{
  // Default constructor
}
//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup(TList *l)
: TNamed()
{
  // Normal constructor
  fFileName = "";
  fList = 0;
  if (!l) return;
  SetName(l->GetName());
  fList = l;
}
//______________________________________________________________________________
AliRsnOutGroup::AliRsnOutGroup(const AliRsnOutGroup &copy)
: TNamed(copy),
  fList(copy.fList)
{
  // Copy constructor
}
//______________________________________________________________________________
AliRsnOutGroup &AliRsnOutGroup::operator=(const AliRsnOutGroup &other)
{
  // Assignment operator
  if(this != &other) {
    fList = other.fList;
  }
  return *this;
}
//______________________________________________________________________________
AliRsnOutGroup::~AliRsnOutGroup()
{
  // Destructor
}
