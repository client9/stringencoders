#!/usr/bin/env python


class Trie(object):
    def __init__(self):
        self.struct = [
            [ ]
        ]

    def dump(self):
        count = 0
        for i in self.struct:
            count += len(i)
            print i
        print len(self.struct)
        print count

    def longest_match(self, s):
        """
        sample code
        """
        leaf = self.struct[0]
        matches = []
        for ch in s:
            newleaf = None
            for node in leaf:
                if ch == node['value']:
                    found = True
                    if node['userdata'] is not None:
                        matches.append(node['userdata'])
                    if node['next'] == 0:
                        return matches
                    newleaf = self.struct[node['next']]
                    break
            if newleaf:
                leaf = newleaf
            else:
                break
        return matches

    def compile(self):
        """
        The current data struct is a a ragged array
        0 - [ ]
        1 - [   ]
        2 - [      ]
        3 - [  ]
        i.e. an array of vectors which are ugly to
        statically allocate in C
        http://stackoverflow.com/questions/1083658/jagged-array-in-c

        value, more, next, [userdata or 0]
        """
        offsets = [ 0 ]
        count = 0
        for leaf in self.struct:
            count += len(leaf)
            offsets.append(count)
        print """
#ifndef _MODP_HTML_NAMED_ENTITIES_H
#define _MODP_HTML_NAMED_ENTITIES_H
#include <string.h>

typedef struct trienode {
    char value;    /* 1 byte -- char value - we deal only in pure ASCII */
    char more;     /* 1 byte -- should be read next value in array (boundary marker) */
    short next;    /* 2 byte -- position of next leaf */
    int codepoint; /* 4 byte -- codepoint or 0 if none */
} trienode_t;

static const trienode_t entities[];
static int html_named_entity_decode(const char* s, size_t len, size_t* consumed);

static int html_named_entity_decode(const char* s, size_t len, size_t* consumed)
{
   int codepoint = 0;
   size_t pos = 0;
   int i = 0;

   *consumed = 0;

   while (pos < len) {
        if (s[pos] == entities[i].value) {
           if (entities[i].codepoint) {
               codepoint = entities[i].codepoint;
               *consumed = pos + 1;
           }
           if (entities[i].next != 0) {
               i = entities[i].next;
               pos += 1;
               continue;
           } else {
               break;
           }
        }
        if (entities[i].more == 0) {
           break;
        }
        ++i;
   }
   return codepoint;
}
/*
#include <stdio.h>
int main(int argc, char* argv[]) {
    size_t consumed;
    int codepoint;
    codepoint = html_named_entity_decode(argv[1], strlen(argv[1]), &consumed);
    printf("cp = %d, consumed = %d\\n", codepoint, consumed);
    return 0;
}
*/
static const trienode_t entities[] = {
"""
        for leaf in self.struct:
            nodelen = 0
            for node in leaf:
                nodelen += 1
                if node['full'] != None:
                    comment = ' /* &' + node['full'] + ' */'
                else:
                    comment = ''

                print "{{ {0}, {1}, {2}, {3} }},{4}".format(ord(node['value']),
                                                            int(nodelen != len(leaf)),
                                                            offsets[node['next']],
                                                            node['userdata'],
                                                            comment
                                                        )
        print "};"
        print "#endif"

    def add(self, s, userdata):
        idx = 0
        leaf = self.struct[0]
        count = 0
        for ch in s[0:-1]:
            #print "CHAR " + i
            key = None
            for node in leaf:
                if ch == node['value']:
                    key = node
                    break
            if key is None:
                #print "key is none"
                newleaf = [ ]
                self.struct.append(newleaf)
                idx = len(self.struct) -1
                leaf.append( {
                    "full": None,
                    "value": ch,
                    "userdata": 0,
                    "next": idx
                } )
                leaf = newleaf
            elif key['next'] != 0:
                # just follow
                #print "got key index of " + str(key[1])
                leaf = self.struct[key['next']]
            else:
                newleaf = [ ]
                self.struct.append(newleaf)
                idx = len(self.struct) -1
                key['next'] = idx
                leaf = newleaf

        key = None
        ch = s[-1]
        for node in leaf:
            if ch == node['value']:
                key = node
                break
        if key is None:
            leaf.append( {
                'full': s,
                'userdata': userdata,
                'next': 0,
                'value': s[-1]
            })
        else:
            key['full'] = s
            key['userdata'] = userdata



import json
with open('entities.json', 'r') as fd:
    obj = json.load(fd)

t = Trie()
total = 0
for name, data in obj.iteritems():
    total += len(name) -1
    t.add(name[1:], data['codepoints'][0])

#for i in ["fo", "fa", "foo"]:
#    t.add(i)

#t.dump()
#print t.longest_match("amp;")

t.compile()

