####################################################################
#
#    This file was generated using Parse::Yapp version 1.01.
#
#        Don't edit this file, use source file instead.
#
#             ANY CHANGE MADE HERE WILL BE LOST !
#
####################################################################
package htplparse;
use vars qw ( @ISA );
use strict;

@ISA= qw ( Parse::Yapp::Driver );
use Parse::Yapp::Driver;



sub new {
        my($class)=shift;
        ref($class)
    and $class=ref($class);

    my($self)=$class->SUPER::new( yyversion => '1.01',
                                  yystates =>
[
	{#State 0
		ACTIONS => {
			'NOT' => 2,
			"(" => 5,
			'STRING' => 6
		},
		GOTOS => {
			'expression' => 1,
			'token' => 3,
			'value' => 4
		}
	},
	{#State 1
		ACTIONS => {
			'' => 7
		}
	},
	{#State 2
		ACTIONS => {
			'NOT' => 2,
			"(" => 5,
			'STRING' => 6
		},
		GOTOS => {
			'token' => 8,
			'value' => 4
		}
	},
	{#State 3
		ACTIONS => {
			'OR' => 9
		},
		DEFAULT => -1
	},
	{#State 4
		ACTIONS => {
			'AND' => 10
		},
		DEFAULT => -3
	},
	{#State 5
		ACTIONS => {
			'NOT' => 2,
			"(" => 5,
			'STRING' => 6
		},
		GOTOS => {
			'expression' => 11,
			'token' => 3,
			'value' => 4
		}
	},
	{#State 6
		ACTIONS => {
			'EQUAL' => 12,
			'NOTEQUAL' => 13,
			"<" => 14,
			">" => 15
		},
		DEFAULT => -6,
		GOTOS => {
			'ratio' => 16
		}
	},
	{#State 7
		DEFAULT => -0
	},
	{#State 8
		DEFAULT => -5
	},
	{#State 9
		ACTIONS => {
			'NOT' => 2,
			"(" => 5,
			'STRING' => 6
		},
		GOTOS => {
			'expression' => 17,
			'token' => 3,
			'value' => 4
		}
	},
	{#State 10
		ACTIONS => {
			'NOT' => 2,
			"(" => 5,
			'STRING' => 6
		},
		GOTOS => {
			'token' => 18,
			'value' => 4
		}
	},
	{#State 11
		ACTIONS => {
			")" => 19
		}
	},
	{#State 12
		DEFAULT => -12
	},
	{#State 13
		DEFAULT => -11
	},
	{#State 14
		DEFAULT => -9
	},
	{#State 15
		DEFAULT => -10
	},
	{#State 16
		ACTIONS => {
			'STRING' => 20
		}
	},
	{#State 17
		DEFAULT => -2
	},
	{#State 18
		DEFAULT => -4
	},
	{#State 19
		DEFAULT => -7
	},
	{#State 20
		DEFAULT => -8
	}
],
                                  yyrules  =>
[
	[#Rule 0
		 '$start', 2, undef
	],
	[#Rule 1
		 'expression', 1,
sub
#line 2 "htplparse.yp"
{$_[1];}
	],
	[#Rule 2
		 'expression', 3,
sub
#line 3 "htplparse.yp"
{['||', $_[1], $_[3]];}
	],
	[#Rule 3
		 'token', 1,
sub
#line 5 "htplparse.yp"
{$_[1];}
	],
	[#Rule 4
		 'token', 3,
sub
#line 6 "htplparse.yp"
{['&&', $_[1], $_[3]];}
	],
	[#Rule 5
		 'token', 2,
sub
#line 7 "htplparse.yp"
{['!', $_[2]];}
	],
	[#Rule 6
		 'value', 1,
sub
#line 9 "htplparse.yp"
{[$_[1]];}
	],
	[#Rule 7
		 'value', 3,
sub
#line 10 "htplparse.yp"
{$_[2];}
	],
	[#Rule 8
		 'value', 3,
sub
#line 11 "htplparse.yp"
{['', $_[2], $_[1], $_[3]];}
	],
	[#Rule 9
		 'ratio', 1, undef
	],
	[#Rule 10
		 'ratio', 1, undef
	],
	[#Rule 11
		 'ratio', 1, undef
	],
	[#Rule 12
		 'ratio', 1, undef
	]
],
                                  @_);
    bless($self,$class);
}

#line 14 "htplparse.yp"


1;
