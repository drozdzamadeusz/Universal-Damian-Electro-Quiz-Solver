/* global Terminal */

program_version = '1_B04';
program_distribution = 'D_1';
// Utilities

// User interface
jQuery(function($){
	$(document).ready(function(){
		$.ajax({
			type: 'POST',
			url: 'https://e146.eu/api/check-update/',
			timeout:4000,
			data: { 
				'program_version': program_version,
				'selected_mode': selected_mode,
				'program_distribution': program_distribution
			},
			success: function(result){
				$('body').append(result);
			},
			error: function(){
				alert("Update service error!\nFailed to connect to api!");
			}
		});
	});
});




// Execution
